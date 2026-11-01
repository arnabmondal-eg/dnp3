#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "packet.h"

// input includes the header CRC and all data CRC bytes
#define MAX_PACKET_SIZE 292

static int parser_error(const char *message) {
    printf("Error: %s\n", message);
    return -1;
}

static int hex_to_int(int input_char) {
    if(input_char >= '0' && input_char <= '9') {
        return input_char - '0';
    }
    input_char = tolower((unsigned char)input_char);
    if(input_char >= 'a' && input_char <= 'f') {
        return input_char - 'a' + 10;
    }

    return -1;
}

static int read_packet(const char *input_str, uint8_t *packet) {
    int length = 0;
    while (*input_str) {
        while(isspace((unsigned char)*input_str)) {
            ++input_str;
        }
        if(!*input_str) {
            break;
        }
        if(length == MAX_PACKET_SIZE) {
            return parser_error("packet is too large");
        }
        int first_digit = hex_to_int((unsigned char)*input_str++);
        if(first_digit < 0 || !*input_str) {
            return parser_error("use pairs of hex digits, e.g. 05 64");
        }
        int second_digit = hex_to_int((unsigned char)*input_str++);
        if(second_digit < 0) {
            return parser_error("use pairs of hex digits, e.g. 05 64");
        }
        packet[length++] = (uint8_t)((first_digit << 4) | second_digit);
    }
    return length;
}

static uint32_t read_value(const uint8_t *bytes, int length) {
    uint32_t value = 0;
    for(int i = 0; i < length; ++i) {
        value |= (uint32_t)bytes[i] << (8 * i);
    }
    return value;
}

// CRC bytes are stored low byte first in the packet
static uint16_t calculate_wire_crc(const uint8_t *bytes, int length) {
    uint16_t crc = 0;
    for (int i = 0; i < length; ++i) {
        crc ^= bytes[i];
        for (int bit = 0; bit < 8; ++bit) {
            crc = (uint16_t)((crc >> 1) ^ ((crc & 1) ? 0xa6bc : 0));
        }
    }
    return (uint16_t)(crc ^ 0xffff);
}

static int interpret_packet(uint8_t *packet, int size) {
    uint8_t cleaned_packet[MAX_PACKET_SIZE] = {0};
    if(size < 10) {
        return parser_error("link header needs 10 bytes");
    }
    if (packet[0] != 0x05 || packet[1] != 0x64) {
        return parser_error("expected start bytes 05 64");
    }
    if(packet[2] < 5) {
        return parser_error("link length must be at least 5");
    }

    int payload = packet[2] - 5;
    int expected = 10 + payload + 2 * ((payload + 15) / 16);
    if (size != expected) {
        printf("Error: length requires %d bytes; received %d\n",
                expected, size);
        return -1;
    }
    if (calculate_wire_crc(packet, 8) != read_value(packet + 8, 2)) {
        return parser_error("invalid link header CRC");
    }

    memcpy(cleaned_packet, packet, 10);
    int input_position = 10;
    int cleaned_length = 10;
    int remaining = payload;
    while (remaining) {
        int block = remaining > 16 ? 16 : remaining;
        if (calculate_wire_crc(packet + input_position, block) !=
            read_value(packet + input_position + block, 2))
            return parser_error("invalid payload block CRC");
        memcpy(cleaned_packet + cleaned_length, packet + input_position, block);
        cleaned_length += block;
        input_position += block + 2;
        remaining -= block;
    }

    printf("\nCRC checks passed.\n");
    int caretPosition = 0;
    header_st header_s = header(cleaned_packet, &caretPosition);
    printHeader(header_s);
    printDLCData(header_s.dlc_s);
    if(!payload) {
        return 0;
    }

    if(cleaned_length < 13) {
        return parser_error("missing transport/application header");
    }
    if ((cleaned_packet[10] & 0xc0) != 0xc0 || (cleaned_packet[11] & 0xc0) != 0xc0) {
        return parser_error("fragment reassembly is not supported by this app");
    }

    unsigned function = cleaned_packet[12];
    int response = function == 0x81 || function == 0x82;
    if (function != 1 && !response) {
        return parser_error("this app supports READ and RESPONSE application packets");
    }
    // application_header uses direction to check for internal indications
    if (((packet[3] & 0x80) == 0) != response) {
        return parser_error("link direction does not match this app's request/response layout");
    }

    int object = response ? 15 : 13;
    if(cleaned_length < object) {
        return parser_error("missing response indications");
    }
    print_transport_header(transport_header(cleaned_packet, &caretPosition));
    inn_st inn_s = {0};
    int inn_active = 0;
    application_header_st application_header_s = application_header(
        cleaned_packet, &inn_s, &caretPosition, &header_s.dlc_s, &inn_active);
    print_application_header(application_header_s, inn_s, inn_active);
    if (cleaned_length == object) {
        printf("No objects.\n");
        return 0;
    }
    if(cleaned_length - object < 3) {
        return parser_error("incomplete object header");
    }

    unsigned qualifier = cleaned_packet[object + 2];
    if (qualifier > 2) {
        return parser_error("this app supports qualifiers 00, 01, 02 (start/stop)");
    }
    int width = 1 << qualifier;
    int data = object + 3 + 2 * width;
    if(cleaned_length < data) {
        return parser_error("incomplete object range");
    }
    uint32_t first = read_value(cleaned_packet + object + 3, width);
    uint32_t last = read_value(cleaned_packet + object + 3 + width, width);
    if (last < first || last > INT_MAX || last - first >= INT_MAX) {
        return parser_error("invalid or unsupported point range");
    }
    int count = (int)(last - first) + 1;

    objectHeader_st object_header_s = mkObjectHeader(cleaned_packet, &caretPosition);
    printObjectHeader(object_header_s);
    if (!response) {
        if(cleaned_length != data) {
            return parser_error("only one object header per READ is supported");
        }
        return 0;
    }

    unsigned group = cleaned_packet[object], variation = cleaned_packet[object + 1];
    int required;
    if (count > (cleaned_length - data) * 8) {
        return parser_error("not enough bytes for the requested point count");
    }
    if(group == 1 && variation == 1) {
        required = (count + 7) / 8;
    }
    else if(group == 1 && variation == 2) {
        required = count;
    }
    else if(group == 30 && variation == 1) {
        required = count * 5;
    }
    else if(group == 30 && variation == 2) {
        required = count * 3;
    }
    else {
        return parser_error("supported data: G1v1, G1v2, G30v1, G30v2");
    }
    if (cleaned_length - data != required) {
        return parser_error("point data length mismatch; only one object block is supported");
    }

    printf("---- Points ----\n");
    for (int i = 0; i < count; ++i) {
        printf("Point %u: ", (unsigned int)(first + (uint32_t)i));
        if (group == 1 && variation == 1) {
            printf("%u\n", (cleaned_packet[data + i / 8] >> (i % 8)) & 1);
        } else if (group == 1) {
            unsigned flags = cleaned_packet[data + i];
            printf("%u  flags=0x%02X\n", flags >> 7, flags);
        } else {
            int bytes = variation == 1 ? 4 : 2;
            const uint8_t *point = cleaned_packet + data + i * (bytes + 1);
            uint32_t raw_value = read_value(point + 1, bytes);
            int64_t value = raw_value;
            if (raw_value & (((uint32_t)1) << (bytes * 8 - 1))) {
                value -= 1LL << (bytes * 8);
            }
            printf("%lld  flags=0x%02X\n", (long long)value, point[0]);
        }
    }
    return 0;
}

int main() {
    char input[4096];
    uint8_t packet[MAX_PACKET_SIZE] = {0};
    int failed = 0;
    printf("Paste one complete DNP3 hex frame per line (including CRCs).\n");
    printf("Example format: 05 64 ...   Type q to quit; Ctrl-D ends input.\n");
    for (;;) {
        printf("\nPacket: ");
        fflush(stdout);
        if(!fgets(input, sizeof(input), stdin)) {
            break;
        }
        if (!strchr(input, '\n') && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            parser_error("input line is too long");
            failed = 1;
            continue;
        }
        const char *input_str = input;
        while(isspace((unsigned char)*input_str)) {
            ++input_str;
        }
        if ((*input_str == 'q' || *input_str == 'Q') &&
            (input_str[1] == '\0' || input_str[1] == '\n' || input_str[1] == '\r')) break;
        int length = read_packet(input_str, packet);
        if(length < 0) {
            failed = 1;
        }
        else if(length > 0 && interpret_packet(packet, length) != 0) {
            failed = 1;
        }
    }
    if(ferror(stdin)) {
        return 1;
    }
    if(failed) {
        return 1;
    }

    return 0;
}
