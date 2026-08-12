#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

#define LOG_ERR    "\033[31m"
#define LOG_WARN "\033[33m"
#define LOG_START "\033[32m"
#define LOG_RESET  "\033[0m"

#define INFO_CONSOLE 0
#define INFO_FILE 1
#define INFO_BOTH 2

#ifndef log_h
#define log_h

static void log_timestamp(FILE*);
static void log_vprintf(FILE *, const char *, va_list);

int log_init(const char *);
FILE* log_get_file();

void log_program_start(const char*);
void log_program_terminate(const char*);
void log_program_crash(const char*);

void log_err(int, const char*);
void log_warn(int, const char*);
void log_info(int, const char*, ...);

#endif