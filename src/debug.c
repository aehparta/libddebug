/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

/******************************************************************************/
/* INCLUDES */
#include "debug.h"


/* If debugging is turned off. */
#ifndef _DEBUG_REC
void *rec_malloc(int x) { return (_mm_malloc(x, 16)); }
void rec_mfree(void *x) { _mm_free(x); }
int rec_allocs(void) { return (-1); }
int rec_frees(void) { return (-1); }
char *rec_dump(void) { return ("Not in debug mode."); }


/* Dont use anything from here if debugging is off. */
#else


/******************************************************************************/
/* VARIABLES */

/** Counter that keeps track of different allocations. */
int rec_allocsn = 0;

/** Counter that keeps track of different frees. */
int rec_freesn = 0;

/** Log buffer. Record everything as text in here. */
char *rec_log = NULL;

/** Size of record log currently. */
int rec_logn = 1;

/** Record loggging semaphore. */
semt rec_sem = SEM_ERROR;

/** Address buffer. */
unsigned int addrs[1024];

/** Enable debug recording or not. */
int debug_enable = 1;


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Initialize allocation record logging.
*/
void rec_init(void)
{
	rec_allocsn = 0;
	rec_freesn = 0;
	rec_log = NULL;
	rec_logn = 1;
	rec_sem = semt_create(1);
	rec_allocsn--;
	memset(addrs, 0, sizeof(addrs));
}


/******************************************************************************/
/**
	Deinitialize allocation record logging.
	Notice! This will NOT free debug record log so that it can be used
	even after calling NET_Quit(). Use only in debug mode, generates
	a memory leak.
*/
void rec_quit(void)
{
	/* Variables. */
	semt t;
	
	t = rec_sem;
	rec_sem = NULL;
	semt_free(t);
	rec_freesn--;
}


/******************************************************************************/
/**
	Add record to log buffer.
	
	@param name Log name identifier.
	@param str Pointer to string to be added.
*/
void rec_add(const char *name, const char *str, void *id, char *file, int line)
{
	/* Variables. */
	char buf[320];
	char *t;
	
	if (rec_sem == NULL || debug_enable == 0)
	{
		return;
	}

	semt_wait(rec_sem);

	sprintf(buf, "0x%08x - %s: %s (%s@%d)\n", (unsigned long)id, name,
	        str, file, line);

	if (rec_log)
	{
		t = realloc(rec_log, rec_logn + strlen(buf));
	}
	else
	{
		t = realloc(rec_log, rec_logn + strlen(buf));
		if (t)
		{
			memset(t, '\0', rec_logn + strlen(buf));
		}
	}
	
	if (t)
	{
		rec_log = t;
		rec_logn += (int)strlen(buf);
		strcat(rec_log, buf);
	}

	semt_post(rec_sem);
}


/******************************************************************************/
/**
	Allocate memory and add debugging record. This is same as malloc()
	but adds DNET internal resource allocation record automatically.

	@param n Amount of memory to be allocated.
	@return Pointer to memory allocated ot NULL on errors.
*/
void *_rec_malloc(unsigned int n, char *file, int line)
{
	/* Variables. */
	void *x;
	char str[320];
	int i;

	/* Allocate. */
	x = _mm_malloc(n, 16);
	rec_allocsn++;
	sprintf(str, "Allocated new memory %d bytes.", n);
	rec_add("malloc", str, x, file, line);
	
	for (i = 0; addrs[i] != NULL && i < 1024; i++);
	addrs[i] = x;
	
	/* Return. */
	return (x);
}


/******************************************************************************/
/**
	Free memory and add debugging record. Same as free(). (see: rec_malloc())
	
	@param x Pointer to memory to be freed.
*/
void _rec_mfree(void *x, char *file, int line)
{
	int i;

	if (x)
	{
		rec_add("free", "Memory freed.", x, file, line);
		_mm_free(x);
		rec_freesn++;
		
		for (i = 0; i < 1024; i++)
		{
			if (addrs[i] == x)
			{
				addrs[i] = NULL;
				break;
			}
		}
		
		if (i >= 1024)
		{
			rec_add("free error", "Tried to free unallocated memory.",
			        x, file, line);
		}
	}
}


/******************************************************************************/
/**
	Add record to DNET internal allocation record. Used example when creating
	new thread or allocating new semaphore. First parameter should be name
	of the resource allocated and same name should be used in rec_free().

	@param name Name of resource just allocated.
	@param pointer Pointer identifier for allocation.
*/
void _rec_alloc(const char *name, void *pointer, char *file, int line)
{
	int i;

	rec_allocsn++;
	rec_add(name, "Resource allocated.", pointer, file, line);

	for (i = 0; addrs[i] != NULL && i < 1024; i++);
	addrs[i] = pointer;
}


/******************************************************************************/
/**
	See rec_alloc().

	@param name Name of resource just freed.
	@param pointer Pointer identifier for free.
*/
void _rec_free(const char *name, void *pointer, char *file, int line)
{
	int i;

	rec_freesn++;
	rec_add(name, "Resource freed.", pointer, file, line);

	for (i = 0; i < 1024; i++)
	{
		if (addrs[i] == pointer)
		{
			addrs[i] = NULL;
			break;
		}

		if (i >= 1024)
		{
			rec_add("free error", "Tried to free unallocated resource.",
			        pointer, file, line);
		}
	}
}


/******************************************************************************/
/**
	Dump current debug records log.

	@return Pointer to log buffer.
*/
char *rec_dump(void)
{
	if (rec_log == NULL)
	{
		return ("Log is empty.");
	}
	
	return (rec_log);
}


/******************************************************************************/
/**
	Returns number of current calls to allocation functions.
	
	@return Number of allocations.
*/
int rec_allocs(void)
{
	return (rec_allocsn);
}


/******************************************************************************/
/**
	Returns number of current calls to free functions.
	
	@return Number of frees.
*/
int rec_frees(void)
{
	return (rec_freesn);
}


/******************************************************************************/
/**
	Returns array to pointers not freed.
*/
unsigned int rec_respointers(void)
{
	return (addrs);
}


/******************************************************************************/
/** Enable debug recording. */
void rec_enable(void)
{
	debug_enable = 1;
}


/******************************************************************************/
/** Disable debug recording. */
void rec_disable(void)
{
	debug_enable = 0;
}

#endif /* Only when debugging. */


/******************************************************************************/
/** Start timing recording. */
void dd_timerec_start(struct timerec *tr)
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	tr->ticks_start = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	tr->ticks = 0;
	tr->sec = 0;
	tr->msec = 0;
}


/******************************************************************************/
/** Get current timing recording. */
void dd_timerec_time(struct timerec *tr)
{
	struct timeval tv;
	uint32_t start;
	
	gettimeofday(&tv, NULL);
	tr->ticks = (tv.tv_sec * 1000 + tv.tv_usec / 1000) - tr->ticks_start;
	tr->sec = tr->ticks / 1000;
	tr->msec = tr->ticks % 1000;
}


/******************************************************************************/
/** Print timing recording. */
void dd_timerec_print(struct timerec *tr, int fd)
{
	struct timeval tv;
	int h, m, s, ms;
	
	gettimeofday(&tv, NULL);
	tr->ticks = (tv.tv_sec * 1000 + tv.tv_usec / 1000) - tr->ticks_start;
	tr->sec = tr->ticks / 1000;
	tr->msec = tr->ticks % 1000;
	h = tr->sec / 3600;
	m = (tr->sec % 3600) / 60;
	s = tr->sec % 60;
	ms = tr->msec;
	printf("%0.2d:%0.2d:%0.2d.%0.3d\n", h, m, s, ms);
}


/******************************************************************************/
/** Return malloc():ed string of data hex presentation. */
char *hexdump(const unsigned char *p, size_t size)
{
	char *hex, *rhex;
	int i;
	size_t len = (size * 2) + size / 2 + 1;
	rhex = hex = malloc(len);
	memset(hex, 0, len);
	for (i = 0; i < size; i += 2)
	{
		sprintf(hex, "%0.2x%0.2x ", (unsigned int)p[i], (unsigned int)p[i + 1]);
		hex += 5;
	}
	if (i % 1)
	{
		sprintf(hex, "%0.2x", (unsigned int)p[i + 1]);
	}
	else
	{
		hex--;
		*hex = '\0';
	}
	return rhex;
}


