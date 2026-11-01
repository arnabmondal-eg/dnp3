# DNP3 Parser

A C project for parsing DNP3 packets and simulating communication between a client and server over TCP. DNP3 is commonly used in SCADA systems.

The project implements a small subset of the protocol. The goal is to understand packet structure and demonstrate requests and responses without implementing the entire specification.

## Features

- Interactive packet parser: paste a hexadecimal packet and inspect its contents.
- Link, transport, application, and object header interpretation.
- Packet length and header/data CRC validation in the interactive parser.
- Binary and analog point decoding.
- TCP client and server with configurable addresses and ports.
- Automatic client polling across configured servers.
- Manual server selection, link reset, and data requests.
- Generated server point values for demonstrations.
- Console output and client/server log files.

## Supported data formats

| Group | Variation | Format |
| --- | --- | --- |
| 1 | 1 | Packed binary input |
| 1 | 2 | Binary input with flags |
| 30 | 1 | 32-bit analog input with flags |
| 30 | 2 | 16-bit analog input with flags |

The interactive parser displays point indices, values, and raw flag bytes where present.

## Requirements

- A C compiler with C11 support, such as GCC or Clang.
- CMake 3.16 or newer.
- Linux, macOS, or a Linux environment through WSL.

Java and a JDK are not required.

### Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential cmake git
```

### macOS

Install Apple's command-line tools, then install CMake using [Homebrew](https://brew.sh/):

```bash
xcode-select --install
brew install cmake
```

If the command-line tools are already installed, skip the first command.

## Build

Clone the repository and build from its root:

```bash
git clone https://github.com/arnabmondal-eg/dnp3.git
cd dnp3

cmake -S backend -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target parser client server --parallel
```

The executables are created in `build/bin/`:

| Executable | Purpose |
| --- | --- |
| `parser` | Interpret pasted DNP3 hex packets |
| `client` | Connect to servers and request data |
| `server` | Generate point data and respond to requests |

Build on each target operating system and architecture to produce its native binaries.

## Packet parser

Run:

```bash
./build/bin/parser
```

Paste one complete packet per line, including the link header CRC and every data block CRC. Use pairs of hexadecimal digits, with or without whitespace. Do not include `0x` prefixes, commas, timestamps, or log messages.

Example READ request for Group 1 Variation 2, points 0 through 2:

```text
05 64 0d c4 00 00 01 00 f3 87 c0 c0 01 01 02 00 00 02 0a 77
```

The parser checks the frame length and CRCs, then prints the supported headers and response point values. It reports invalid or unsupported input and waits for another packet.

Type `q` or press `Ctrl+D` to exit.

### Read packets from a file

A text file can contain one complete hex packet per line:

```bash
./build/bin/parser < packets.txt
```

The program returns exit code `1` if any input fails parsing or validation; otherwise, it returns `0`. Prompts and diagnostic messages are also printed when reading from a file.

### Parser scope

- Link-only frames and application READ (`0x01`), RESPONSE (`0x81`), and UNSOLICITED RESPONSE (`0x82`) packets.
- One object block per packet.
- Start/stop qualifiers `0x00`, `0x01`, and `0x02`.
- A maximum complete frame size of 292 bytes.
- Complete, unfragmented transport and application messages.
- Requests with the link direction bit set and responses with it cleared, matching the current library's header layout.

Multiple object blocks, other qualifier forms, and fragment reassembly are not supported by the interactive parser.

## Client/server demo

Run the server and client in separate terminals, starting the server first.

### 1. Start a server

```bash
./build/bin/server 48188
```

The port argument is optional; the default is `48188`.

Enter the following configuration for a small binary-input demo:

| Prompt | Value |
| --- | --- |
| Data point type | `1` |
| Data point variation | `2` |
| Point start, inclusive | `0` |
| Point end, inclusive | `7` |

The server waits for a client, sends its configuration, and generates point values when processing requests. Each server process accepts one client connection.

### 2. Start a client

The client accepts:

```text
client [number_of_servers] [mode]
```

| Argument | Meaning | Default |
| --- | --- | --- |
| `number_of_servers` | Number of server connections to configure, up to 128 | `1` |
| `mode` | `0` for automatic polling; a nonzero value for manual selection | `0` |

For one server in automatic mode:

```bash
./build/bin/client 1 0
```

When prompted, enter port `48188` and address `0` for localhost. You can also enter an IPv4 address such as `127.0.0.1`.

The client first sends link-reset requests, then repeatedly requests data from each configured server. It pauses for one second before each data request and prints replies as hexadecimal packets.

Press `Ctrl+C` to stop automatic polling.

### Manual mode

```bash
./build/bin/client 1 1
```

Configure the port and address, then select server `0`. Server indices start at zero.

The request menu provides:

```text
1. Reset Link
2. Request Data
3. Back
```

Choose `3` to return to server selection, then enter `-1` to exit.

### Multiple servers

Start each server in a separate terminal on a different port:

```bash
./build/bin/server 48188
./build/bin/server 48189
```

Then start a client with two connections:

```bash
./build/bin/client 2 0
```

Enter the corresponding port and address for each connection. Each server can use a different supported group, variation, and point range.

### Interpret a received packet

The client prints raw replies. Copy one complete reply into the `parser` program to inspect its headers and supported point values.

If the packet is printed across several lines, join those hex bytes into one line before pasting. Include all CRC bytes and exclude surrounding log text.

## Logs

The client and server create a `log/` directory relative to the directory from which they are launched.

- `log/client_log.txt`
- `log/server_log.txt`

Automatic client mode also writes received raw packets to the client log. The standalone parser prints its output to the terminal.

## Project structure

```text
dnp3/
├── .vscode/                 Editor configuration
├── backend/
│   ├── CMakeLists.txt       Build configuration
│   ├── parser-app/src/      Interactive packet parser
│   ├── client/
│   │   ├── inc/             Client headers
│   │   └── src/             Client implementation
│   ├── server/
│   │   ├── inc/             Server headers
│   │   └── src/             Server implementation
│   └── libraries/
│       ├── parser/          Packet structures, parsing, and serialization
│       ├── network/         Socket setup and connection helpers
│       ├── common/          Shared connection and packet I/O helpers
│       └── logging/         Logging helpers
├── build/                   Generated by the build commands above
│   └── bin/                 parser, client, and server
└── log/                     Created when running the client or server
```

## Current limitations

This is a partial protocol implementation with known correctness issues still being addressed. Support for a data format does not imply full DNP3 conformance.

The client/server demo uses a project-specific configuration exchange before sending DNP3 frames, so it does not establish interoperability with other DNP3 implementations. The interactive parser's CRC and length checks are separate from the live client/server receive path.

Server point storage and the size of a single link frame limit request sizes. Large configurations are not automatically split into multiple frames. Use small point ranges for demonstrations.
