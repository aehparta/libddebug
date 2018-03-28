/*
 * DDebuglib
 *
 * License: GNU/GPL, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

#ifndef ARRAY3_H
#define ARRAY3_H


/******************************************************************************/
/* INCLUDES */
#include <stdlib.h>
#include <ddebug/synchro.h>


/******************************************************************************/
/* STRUCTS */
struct array3
{
	void **p;
	size_t size_bytes;
	size_t size_x;
	size_t size_y;
	size_t size_z;
	lock_t lock;
};


/******************************************************************************/
/* INLINE FUNCTIONS */

/**
 * Create 3 dimensional array.
 */
static inline struct array3 *v3_new(size_t size_x, size_t size_y, size_t size_z)
{
	struct array3 *v;
	
	v = (struct array3 *)malloc(sizeof(*v));
	if (!v) return NULL;
	memset(v, 0, sizeof(*v));
	if (lock_init(&v->lock))
	{
		free(v);
		return NULL;
	}
	v->size_bytes = size_x * size_y * size_z * sizeof(*v->p);
	v->p = (void **)malloc(v->size_bytes);
	if (!v->p)
	{
		lock_destroy(&v->lock);
		free(v);
		return NULL;
	}
	memset(v->p, 0, v->size_bytes);
	v->size_x = size_x;
	v->size_y = size_y;
	v->size_z = size_z;
	
	return v;
}


/**
 * Set pointer in 3 dimensional array.
 */
static inline void v3_set(struct array3 *v, size_t x, size_t y, size_t z, void *p)
{
	size_t index;
	
	if (x < 0 || x >= v->size_x) return;
	if (y < 0 || y >= v->size_y) return;
	if (z < 0 || z >= v->size_z) return;
	
	lock_write(&v->lock);
	index = x + (y * v->size_x) + (z * v->size_x * v->size_y);
	v->p[index] = p;
	lock_unlock(&v->lock);
}


/**
 * Get pointer from 3 dimensional array.
 */
static inline void *v3_get(struct array3 *v, size_t x, size_t y, size_t z)
{
	size_t index;
	void *p = NULL;
	
	if (x < 0 || x >= v->size_x) return NULL;
	if (y < 0 || y >= v->size_y) return NULL;
	if (z < 0 || z >= v->size_z) return NULL;
	
	lock_read(&v->lock);
	index = x + (y * v->size_x) + (z * v->size_x * v->size_y);
	p = v->p[index];
	lock_unlock(&v->lock);
	return p;
}


/**
 * Copy from array to array within x-axis.
 * Destination array size_x must be equal or more than source.
 */
static inline void v3_cpx(struct array3 *dst, size_t dst_y, size_t dst_z,
                          struct array3 *src, size_t src_y, size_t src_z)
{
	size_t index_dst;
	size_t index_src;
	
	if (dst_y < 0 || dst_y >= dst->size_y) return;
	if (dst_z < 0 || dst_z >= dst->size_z) return;
	if (src_y < 0 || src_y >= src->size_y) return;
	if (src_z < 0 || src_z >= src->size_z) return;
	if (dst->size_x < src->size_x) return;
	
	lock_write(&dst->lock);
	index_dst = (dst_y * dst->size_x) + (dst_z * dst->size_x * dst->size_y);
	index_src = (src_y * src->size_x) + (src_z * src->size_x * src->size_y);
	memmove(&dst->p[index_dst], &src->p[index_src], src->size_x * sizeof(*dst->p));
	lock_unlock(&dst->lock);
}


/**
 * Clear array within x-axis.
 */
static inline void v3_clrx(struct array3 *v, size_t y, size_t z)
{
	size_t index;
	
	if (y < 0 || y >= v->size_y) return;
	if (z < 0 || z >= v->size_z) return;
	
	lock_write(&v->lock);
	index = (y * v->size_x) + (z * v->size_x * v->size_y);
	memset(&v->p[index], 0, v->size_x * sizeof(*v->p));
	lock_unlock(&v->lock);
}


/**
 * Clear array within y-axis.
 */
static inline void v3_clry(struct array3 *v, size_t z)
{
	size_t index;
	
	if (z < 0 || z >= v->size_z) return;
	
	lock_write(&v->lock);
	index = (z * v->size_x * v->size_y);
	memset(&v->p[index], 0, v->size_x * v->size_y * sizeof(*v->p));
	lock_unlock(&v->lock);
}


/**
 * Clear array.
 */
static inline void v3_clr(struct array3 *v)
{
	lock_write(&v->lock);
	memset(v->p, 0, v->size_x * v->size_y * v->size_z * sizeof(*v->p));
	lock_unlock(&v->lock);
}


/**
 * Free 3 dimensional array.
 */
static inline void v3_free(struct array3 *v)
{
	if (v)
	{
		lock_write(&v->lock);
		free(v->p);
		lock_destroy(&v->lock);
		free(v);
	}
}


/******************************************************************************/
/* FUNCTION DEFINITIONS */


#endif /* X_H - END OF HEADER FILE */
/******************************************************************************/

