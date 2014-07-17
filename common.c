/*
 *  common.c
 *  
 *  common functions
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>

#include "common.h"

static int log_write(const char *data, int size)
{
    int succ_size = 0;

    if (size <= 0)
        return 0;

    // write data
    succ_size = fwrite(data, 1, size, stderr);

    if (succ_size <= 0)
        return 0;
    return 1;
}

static void log_doit(const char *filename, int line, const char *function, const char *fmt, va_list args)
{
    struct tm tm_now;
    struct timeval tv_now;
    char buff[128];

    gettimeofday(&tv_now, NULL);
    localtime_r(&(tv_now.tv_sec), &tm_now);

    snprintf(buff, sizeof(buff), "[%04d-%02d-%02d %02d:%02d:%02d] ",tm_now.tm_year + 1900,
             tm_now.tm_mon + 1, tm_now.tm_mday, tm_now.tm_hour, 
             tm_now.tm_min, tm_now.tm_sec);
    log_write(buff, strlen(buff));


    snprintf(buff, sizeof(buff), "<%s (%s:%d)>: ", 
             filename, function, line);
    log_write(buff, strlen(buff));

    vsnprintf(buff, sizeof(buff), fmt, args);
    while (buff[strlen(buff)-1] == '\n')
        buff[strlen(buff)-1] = '\0';
    log_write(buff, strlen(buff));
    log_write("\n", 1);

    fflush(stderr);
}

void log_message(const char *filename, int line, const char *function, const char *fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    log_doit(filename, line, function, fmt, va);
    va_end(va);
}

void *rlc_alloc(const char *filename, int line, const char *function, int size)
{
    void * ptr;
    ptr = calloc(size, 1);
    if (NULL == ptr) {
        log_message(filename, line, function, "get memory error");
    }
    return ptr;
}
