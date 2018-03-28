/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

/******************************************************************************/
/* INCLUDES */
#include "synchro.h"


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/** Get ticks (milliseconds) from program start. */
long get_ticks(void)
{
	long ticks;
	struct timeval now;
	gettimeofday(&now, NULL);
	ticks = now.tv_sec * 1000l;
	ticks += now.tv_usec / 1000l;
	return ticks;
};


/******************************************************************************/
/**
	Creates new semaphore with given start value.
	Semaphores are not shared within processes, when
	created in here.
*/
semt semt_create(int n)
{
	sem_t *hsem;
	int err;

	/*
		Little things to do for creating semaphore...
		First allocate memory, then initialize semaphore,
		but remember to release semaphore memory after
		semaphore destroy.
		Check errors too.
	*/
	hsem = (semt)malloc(sizeof(sem_t)); // rec_malloc(sizeof(sem_t));
	if (!hsem)
	{
		return (SEM_ERROR);
	}
	
	err = sem_init(hsem, 0, n);
	if (err)
	{
		free(hsem); // rec_mfree(hsem);
		return (SEM_ERROR);
	}

	rec_alloc("semaphore", hsem);

	return hsem;
}



/******************************************************************************/
/**
	Decrease semaphore value.
*/
int semt_wait(semt hsem)
{
	int err = 0;

	/* Try locking semaphore. */
	err = sem_wait(hsem);
	
	return err;
}


/******************************************************************************/
/**
	Decrease semaphore value with timeout.
	
	@return 0 on success,
	        1 on timeout,
	        -1 on errors.
*/
int semt_wait_timeout(semt hsem, unsigned long timeout)
{
	int err = -1;
	long start;
	
	/* Wait for semaphore. */
	start = get_ticks();
	while ((err = sem_trywait(hsem)) != 0)
	{
		if (get_ticks() > (start + timeout))
		{
			err = -1;
			break;
		}
		usleep(1000);
	}

	return err;
}


/******************************************************************************/
/**
	Increase semaphore value.
*/
int semt_post(semt hsem)
{
//	int err = 0;

	/* Increase value. */
	sem_post(hsem);
	
	return 0;
}



/******************************************************************************/
/**
	Release semaphore.
*/
void semt_free(semt hsem)
{
	/* Destroy semaphore and free memory allocated for it. */
	sem_destroy(hsem);
	free(hsem); // rec_mfree(hsem);
	rec_free("semaphore", hsem);
}


/******************************************************************************/
/**
	Initializes new read-write-lock.
*/
int lock_init(lock_t *lock_var)
{
	int err = 0;

	pthread_mutexattr_t p_attr;
	pthread_mutexattr_init(&p_attr);
	pthread_mutexattr_settype(&p_attr, PTHREAD_MUTEX_RECURSIVE_NP);
	err = pthread_mutex_init(lock_var, &p_attr);
	pthread_mutexattr_destroy(&p_attr);
//	err = pthread_rwlock_init(lock_var, NULL);

out_err:
	return err;
}


/******************************************************************************/
/**
	Initializes new read-write-lock.
*/
int lock_destroy(lock_t *lock_var)
{
	int err = 0;

	err = pthread_mutex_destroy(lock_var);
//	err = pthread_rwlock_destroy(lock_var);

out_err:
	return err;
}


/******************************************************************************/
/**
	Do read lock.
*/
int lock_read(lock_t *lock_var)
{
	int err = 0;

	err = pthread_mutex_lock(lock_var);
// 	err = pthread_rwlock_rdlock(lock_var);

out_err:
	return err;
}


/******************************************************************************/
/**
	Do write lock.
*/
int lock_write(lock_t *lock_var)
{
	int err = 0;

	err = pthread_mutex_lock(lock_var);
// 	err = pthread_rwlock_wrlock(lock_var);

out_err:
	return err;
}


/******************************************************************************/
/**
	Unlock.
*/
int lock_unlock(lock_t *lock_var)
{
	int err = 0;

	err = pthread_mutex_unlock(lock_var);
// 	err = pthread_rwlock_unlock(lock_var);

out_err:
	return err;
}


