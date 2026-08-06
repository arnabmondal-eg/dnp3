#include "log.h"

static void log_vprint(FILE *stream, const char *message, va_list args) {
    log_timestamp(stream);
    vfprintf(stream, message, args);
}

void log_timestamp(FILE *fptr) {
    time_t currentTime;
    struct tm *t;

    time(&currentTime);
    t = localtime(&currentTime);

    if (fptr != NULL) {
        fprintf(fptr,
            "[%02d:%02d:%02d] ",

            t -> tm_hour,
            t -> tm_min,
            t -> tm_sec
        );
    }
}

void log_program_start(const char *message, FILE *fptr) {
    time_t currentTime;
    time(&currentTime);

    fprintf(stdout, LOG_START "\nStarted %s" LOG_RESET" at %s", message, ctime(&currentTime));
    fprintf(fptr, "\nStarted %s at %s", message, ctime(&currentTime));

    return;
}

void log_program_terminate(const char *message, FILE *fptr) {
    time_t currentTime;
    time(&currentTime);

    fprintf(stdout, LOG_START "\nEnded %s" LOG_RESET" at %s", message, ctime(&currentTime));
    fprintf(fptr, "\nEnded %s at %s", message, ctime(&currentTime));

    return;
}

void log_program_crash(const char *message, FILE *fptr) {}


/* TODO: Add ANSI Color escape codes */

// Log Error to File and Console
void log_err(int val_errno, const char *message, FILE *fptr) {

    fprintf(stderr, LOG_ERR);   // paint the terminal in blood (color text red)
    fprintf(stderr, "\n\t~~~~ FATAL ERROR DETECTED ~~~~\n");
    log_timestamp(stderr);
    fprintf(stderr, "[%s/ERROR]: %s\n" LOG_RESET, message, strerror(val_errno));

    if (fptr != NULL) {
        fprintf(fptr, "\n\t~~~~ FATAL ERROR DETECTED ~~~~\n");
        log_timestamp(fptr);
        fprintf(fptr, "%s: %s\n", message, strerror(val_errno));
    }
    
    fflush(stderr);
    if(fptr != NULL) fflush(fptr);

    return;
}

/* Log Warrning to File and Console*/
void log_warn(int val_errno, const char *message, FILE *fptr) {
    log_timestamp(stderr);
    fprintf(stderr, LOG_WARN "%s: %s\n" LOG_RESET, message, strerror(val_errno));    // console

    if (fptr != NULL) {
        log_timestamp(fptr);
        fprintf(fptr, "%s: %s", message, strerror(val_errno));    // file
    }

    fflush(stderr);
    if(fptr != NULL) fflush(fptr);
    
    return;
}

/* Log Info to File, Console, or Both*/
void log_info(FILE *fptr, int type, const char *message, ...) {
    //TODO: Allow this to accepts args like printf
    va_list parameters;
    va_start(parameters, message);

    if (type == INFO_CONSOLE || type == INFO_BOTH) {
        va_list console_cpy;
        va_copy(console_cpy, parameters);

        log_vprint(stdout, message, console_cpy);

        va_end(console_cpy);
    }
    else if((type == INFO_FILE || type == INFO_BOTH) && fptr != NULL) {
        va_list file_cpy;
        va_copy(file_cpy, parameters);

        log_vprint(fptr, message, file_cpy);

        va_end(file_cpy);
    }

    va_end(parameters);

    return;
}