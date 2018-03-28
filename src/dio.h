/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

#ifndef _DIO_H
#define _DIO_H


/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debuglib.h"


/******************************************************************************/
enum {
    DIO_TYPE_FILE = 0,
    DIO_TYPE_MEM = 1,
};


/******************************************************************************/
struct dio_object {
    int type;
    FILE *f;
    unsigned char *data;
    size_t current;
    size_t len;
};
typedef struct dio_object DIO;


/******************************************************************************/
/**
 * Open file as DIO-object. Same as fopen().
 *
 * @param file filename
 * @param mode open mode
 * @return DIO-handle
 */
struct dio_object *dio_fopen(const char *file, const char *mode);

/**
 * Open memory as DIO-object.
 *
 * @param data pointer to memory to be used
 * @param len data length
 * @param dup 0 if the caller has allocated the memory for this function,
 *            1 if the memory should be duplicated
 * @return DIO-handle
 */
struct dio_object *dio_mopen(unsigned char *data, size_t len, int dup);

/**
 * Close DIO-handle.
 *
 * @param dio DIO-handle
 * @return 0 (might return error in some rare cases)
 */
int dio_close(struct dio_object *dio);

/**
 * Get one character from DIO-stream and return it as int. Same as getc().
 *
 * @param dio DIO-handle
 * @return character as int
 */
int dio_getc(struct dio_object *dio);

/**
 * Put one character back to DIO-stream. Same as ungetc().
 *
 * @param c character to put back into stream
 * @param dio DIO-handle
 * @return c on success, EOF on errors
 */
int dio_ungetc(int c, struct dio_object *dio);

/**
 * Read characters from DIO-stream until delim is found.
 * For more usage, see getdelim() (man 3 getdelim).
 *
 * @param lineptr NULL or already allocated buffer where to store characters
 * @param n if lineptr was previously allocated, pointer to size here
 * @param delim character to search
 * @param dio DIO-handle
 * @return length of data read
 */
ssize_t dio_getdelim(char **lineptr, size_t *n, int delim, struct dio_object *dio);


#endif /* _DIO_H */
/******************************************************************************/

