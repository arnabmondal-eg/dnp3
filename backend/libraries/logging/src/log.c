#include "log.h"

static FILE *log_file = NULL;

static void log_vprint(FILE *stream, const char *message, va_list args) {
    log_timestamp(stream);
    vfprintf(stream, message, args);
}

static void log_timestamp(FILE *stream) {
    time_t currentTime;
    struct tm *t;

    time(&currentTime);
    t = localtime(&currentTime);

    if (stream != NULL) {
        fprintf(stream,
            "[%02d:%02d:%02d] ",

            t -> tm_hour,
            t -> tm_min,
            t -> tm_sec
        );
    }
}

int log_init(const char *file_path) {
    log_file = fopen(file_path, "a");

    if(log_file == NULL) {
        log_err(errno, "log/log_init");
        return -1;
    }

    return 0;
}

FILE* log_get_file() {
    return log_file;
}

void log_program_start(const char *message) {
    time_t currentTime;
    time(&currentTime);

    fprintf(stdout, LOG_START "\nStarted %s" LOG_RESET" at %s", message, ctime(&currentTime));
    fprintf(log_file, "\nStarted %s at %s", message, ctime(&currentTime));

    return;
}

void log_program_terminate(const char *message) {
    time_t currentTime;
    time(&currentTime);

    fprintf(stdout, LOG_START "\nEnded %s" LOG_RESET" at %s", message, ctime(&currentTime));
    fprintf(log_file, "\nEnded %s at %s", message, ctime(&currentTime));

    fclose(log_file);

    return;
}

void log_program_crash(const char *message) {}


/* TODO: Add ANSI Color escape codes */

// Log Error to File and Console
void log_err(int val_errno, const char *message) {

    fprintf(stderr, LOG_ERR);   // paint the terminal in blood (color text red)
    fprintf(stderr, "\n\t~~~~ FATAL ERROR DETECTED ~~~~\n");
    log_timestamp(stderr);
    fprintf(stderr, "[%s/ERROR]: %s\n" LOG_RESET, message, strerror(val_errno));

    if (log_file != NULL) {
        fprintf(log_file, "\n\t~~~~ FATAL ERROR DETECTED ~~~~\n");
        log_timestamp(log_file);
        fprintf(log_file, "%s: %s\n", message, strerror(val_errno));
    }
    
    fflush(stderr);
    if(log_file != NULL) fflush(log_file);

    return;
}

/* Log Warrning to File and Console*/
void log_warn(int val_errno, const char *message) {
    log_timestamp(stderr);
    fprintf(stderr, LOG_WARN "%s: %s\n" LOG_RESET, message, strerror(val_errno));    // console

    if (log_file != NULL) {
        log_timestamp(log_file);
        fprintf(log_file, "%s: %s", message, strerror(val_errno));    // file
    }

    fflush(stderr);
    if(log_file != NULL) fflush(log_file);
    
    return;
}

/* Log Info to File, Console, or Both*/
void log_info(int type, const char *message, ...) {
    //TODO: Allow this to accepts args like printf
    va_list parameters;
    va_start(parameters, message);

    if (type == INFO_CONSOLE || type == INFO_BOTH) {
        va_list console_cpy;
        va_copy(console_cpy, parameters);

        log_vprint(stdout, message, console_cpy);

        va_end(console_cpy);
    }
    else if((type == INFO_FILE || type == INFO_BOTH) && log_file != NULL) {
        va_list file_cpy;
        va_copy(file_cpy, parameters);

        log_vprint(log_file, message, file_cpy);

        va_end(file_cpy);
    }

    va_end(parameters);

    return;
}