/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

/******************************************************************************/
#include "dio.h"


/******************************************************************************/
struct dio_object *dio_fopen(const char *file, const char *mode)
{
    int err = 0;
    struct dio_object *dio = NULL;

    DD_SALLOC(dio);
    dio->type = DIO_TYPE_FILE;
    dio->f = fopen(file, mode);
    if (!dio->f) {
        free(dio);
        return NULL;
    }

out_err:
    return dio;
}


/******************************************************************************/
struct dio_object *dio_mopen(unsigned char *data, size_t len, int dup)
{
    int err = 0;
    struct dio_object *dio = NULL;

    DD_SALLOC(dio);
    dio->type = DIO_TYPE_MEM;
    if (dup) {
        dio->data = malloc(len);
        if (!dio->data) {
            free(dio);
            return NULL;
        }
        memcpy(dio->data, data, len);
        dio->len = len;
    } else {
        dio->data = data;
        dio->len = len;
    }

out_err:
    return dio;
}


/******************************************************************************/
int dio_close(struct dio_object *dio)
{
    int err = 0;

    if (dio->type == DIO_TYPE_FILE) {
        err = fclose(dio->f);
        free(dio);
    } else if (dio->type == DIO_TYPE_MEM) {
        free(dio->data);
        free(dio);
    }

out_err:
    return err;
}


/******************************************************************************/
int dio_getc(struct dio_object *dio)
{
    if (dio->type == DIO_TYPE_FILE) {
        return getc(dio->f);
    } else if (dio->type == DIO_TYPE_MEM) {
        if (dio->current >= dio->len) {
            return EOF;
        } else {
            dio->current++;
            return (int)dio->data[dio->current - 1];
        }
    }

    return EOF;
}


/******************************************************************************/
int dio_ungetc(int c, struct dio_object *dio)
{
    if (dio->type == DIO_TYPE_FILE) {
        return ungetc(c, dio->f);
    } else if (dio->type == DIO_TYPE_MEM) {
        if (dio->current <= 0) {
            return EOF;
        } else {
            dio->current--;
            dio->data[dio->current] = (unsigned char)c;
            return c;
        }
    }

    return EOF;
}


/******************************************************************************/
ssize_t dio_getdelim(char **lineptr, size_t *n, int delim, struct dio_object *dio)
{
    if (dio->type == DIO_TYPE_FILE) {
        return getdelim(lineptr, n, delim, dio->f);
    } else if (dio->type == DIO_TYPE_MEM) {
        size_t l = dio->current;
        while (1) {
            if (l >= dio->len) {
                dio->current = dio->len;
                return -1;
            }
            if (((int)dio->data[l]) == delim) {
                l++;
                break;
            }
            l++;
        }
        if (*lineptr) free(*lineptr);
        *lineptr = malloc(l - dio->current + 1);
        if (!*lineptr) return -1;
        memset(*lineptr, 0, l - dio->current + 1);
        memcpy(*lineptr, dio->data + dio->current, l - dio->current);
        dio->current = *n = l;
        return l;
    }

    return -1;
}


