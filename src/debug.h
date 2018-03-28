/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

#ifndef DEBUG_H
#define DEBUG_H

/******************************************************************************/
/* INCLUDES */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mm_malloc.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>

#include "synchro.h"


/******************************************************************************/
/* DEFINES */
#ifdef _WIN32
#define DLLEXP __declspec(dllexport)
#else
#define DLLEXP
#endif


/** Macro for recording thread CPU consume. */
#ifdef _DEBUG__
/* UNFINISHED: TODO! (or maybe not)... */
#define CPU_TIME_RECORD \
{ \
	static long _cpu_time_s = -10000, _cpu_time_us = -10000; \
	static long _cpu_print = -1;
	struct timeval _cpu_now; gettimeofday(&_cpu_now, NULL); \
	if (_cpu_time_s == -10000) { \
	    _cpu_time_s = now.tv_sec; _cpu_time_us = now.tv_usec; } \
	if (_cpu_print < (_cpu_time_s * 1000l + _cpu_time_us / 1000l)) { \
	    _cpu_print = _cpu_time_s * 1000l + _cpu_time_us / 1000l + 1000; } \
}
#else
#define CPU_TIME_RECORD
#endif

struct timerec
{
	uint64_t ticks_start;
	uint64_t ticks;
	uint64_t sec;
	uint64_t msec;
};


/******************************************************************************/
/* FUNCTION DEFINITIONS */
const char *create_guid(void);

#ifdef _DEBUG_REC
void rec_init(void);
void rec_quit(void);
void rec_add(const char *, const char *, void *, char *, int);
void DLLEXP *_rec_malloc(unsigned int, char *, int);
void DLLEXP _rec_mfree(void *, char *, int);
void DLLEXP _rec_alloc(const char *, void *, char *, int);
void DLLEXP _rec_free(const char *, void *, char *, int);
#define rec_malloc(n) _rec_malloc(n, __FILE__, __LINE__)
#define rec_mfree(n) _rec_mfree(n, __FILE__, __LINE__)
#define rec_alloc(n, m) _rec_alloc(n, m, __FILE__, __LINE__)
#define rec_free(n, m) _rec_free(n, m, __FILE__, __LINE__)
char *rec_dump(void);
int rec_allocs(void);
int rec_frees(void);
unsigned int rec_respointers(void);

void rec_enable(void);
void rec_disable(void);

#else
#define rec_init()
#define rec_quit()
#define rec_add(x,y,z)
#define rec_alloc(x,y)
#define rec_free(x,y)

#define rec_enable()
#define rec_disable()

void *rec_malloc(int x);
void rec_mfree(void *x);
int rec_allocs(void);
int rec_frees(void);
char *rec_dump(void);

#endif

void dd_timerec_start(struct timerec *tr);
void dd_timerec_time(struct timerec *tr);
void dd_timerec_print(struct timerec *tr, int fd);
char *hexdump(const unsigned char *p, size_t size);


#endif /* END OF HEADER FILE */
/******************************************************************************/

