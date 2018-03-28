/*
 * DDEBUGLIB
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

#ifndef DEBUGLIB_H
#define DEBUGLIB_H


#include <string.h>
#include <errno.h>

#include "debug.h"
#include "dlog.h"
#include "synchro.h"
#include "linkedlist.h"
#include "dio.h"


/* allocate memory for struct, use IF_ERR() to report errors and set memory to zero */
#define DD_SALLOC(p_item) \
do { \
	p_item = malloc(sizeof(*p_item)); \
	IF_ERR(p_item == NULL, -1, "malloc() failed: %s", strerror(errno)); \
	memset(p_item, 0, sizeof(*p_item)); \
} while (0)


/* allocate memory, use IF_ERR() to report errors and set memory to zero */
#define DD_ALLOC(p_item, size) \
do { \
	p_item = malloc((size)); \
	IF_ERR(p_item == NULL, -1, "malloc() failed: %s", strerror(errno)); \
	memset(p_item, 0, (size)); \
} while (0)


/* allocate memory for string and copy given string to it, use IF_ERR() to report errors */
#define DD_STRDUP(p_item, p_dup) \
do { \
	p_item = malloc(strlen(p_dup) + 1); \
	IF_ERR(p_item == NULL, -1, "strdup failed on malloc(): %s", strerror(errno)); \
	memset(p_item, 0, strlen(p_dup) + 1); \
	strcpy(p_item, p_dup); \
} while (0)


#endif /* DEBUGLIB_H */
/******************************************************************************/

