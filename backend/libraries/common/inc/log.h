#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>

#define LOG_ERR    "\033[31m"
#define LOG_WARN "\033[33m"
#define LOG_START "\033[32m"
#define LOG_RESET  "\033[0m"

#define INFO_CONSOLE 0
#define INFO_FILE 1
#define INFO_BOTH 2

#ifndef log_h
#define log_h

void log_timestamp(FILE*);

void log_program_start(const char*, FILE*);
void log_program_terminate(const char*, FILE*);
void log_program_crash(const char*, FILE*);

void log_err(int, const char*, FILE*);
void log_warn(int, const char*, FILE*);
void log_info(FILE*, int, const char*, ...);

#endif