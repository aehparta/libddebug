/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

#ifndef SYNCHRO_H
#define SYNCHRO_H

/******************************************************************************/
/* DEFINES */


/******************************************************************************/
/* INCLUDES */
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>

#include "debug.h"

#define semt sem_t *
#define SEM_ERROR NULL
#define lock_t pthread_mutex_t
//#define lock_t pthread_rwlock_t


/*
#define LOCK_WRITE(_lock) \
do { \
	int _i, _err = 0; \
	for (_i = 500; _i > 0; _i--) \
	{ \
		_err = pthread_mutex_trylock(_lock); \
		if (_err == 0) break; \
		usleep(10000); \
	} \
	if (_err != 0) \
	{ \
		ERROR_MSG("lock failed"); \
		abort(); \
	} \
} while (0)
*/


/******************************************************************************/
/* FUNCTION DEFINITIONS */
long get_ticks(void);

semt semt_create(int);
int semt_wait(semt);
int semt_wait_timeout(semt, unsigned long);
int semt_post(semt);
void semt_free(semt);

int lock_init(lock_t *);
int lock_destroy(lock_t *);
int lock_read(lock_t *);
int lock_write(lock_t *);
int lock_unlock(lock_t *);


#endif /* END OF HEADER FILE */
/******************************************************************************/

