/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

#ifndef STRLENS_H
#define STRLENS_H


/******************************************************************************/
/* INCLUDES */


/******************************************************************************/
/* DEFINES */

/** Maximum length of normal string. */
#define MAX_STRING			260

/** Length of short string. */
#define SHORT_STRING		64

/** Length of long string. */
#define LONG_STRING			1024

/** Length of long string. */
#define HUGE_STRING			4096

/** Maximum length of path and filename string. */
#ifndef MAX_PATH
#define MAX_PATH			1024
#endif

/** Invalid characters name string. */
#define NAME_INVALID_CHARS			"<>:;/\\=*^~`?\"#%&()!@${[]}+-,.| "

/** Invalid characters for value. */
#define VALUE_INVALID_CHARS			""

/** This is the character, which should precede every comment. */
#define COMMENT_CHARACTER			'#'


/**
	This macro is for copying max string. It sets NULL characters and so on.
	strncpy() does not always do this properly, so this macro is here.
	Actually, when using this macro, the buffer being destination, must
	have MAX_STRING + 1 size.
*/
#define STRCPY(dst, src) \
do { \
	strncpy(dst, src, MAX_STRING); \
	dst[MAX_STRING - 1] = '\0'; \
} while (0)

/** This macro is for copying path string, see STRINGCPY for more info. */
#define PATHCPY(dst, src) \
do { \
	strncpy(dst, src, MAX_PATH); \
	dst[MAX_PATH - 1] = '\0'; \
} while (0)

/** This macro is for copying huge string, see STRINGCPY for more info. */
#define HUGESTRCPY(dst, src) \
do { \
	strncpy(dst, src, HUGE_STRING); \
	dst[HUGE_STRING - 1] = '\0'; \
} while (0)

/** This macro is for copying short string, see STRINGCPY for more info. */
#define SHORTSTRCPY(dst, src) \
do { \
	strncpy(dst, src, SHORT_STRING); \
	dst[SHORT_STRING - 1] = '\0'; \
} while (0)

/** This macro is for copying long string, see STRINGCPY for more info. */
#define LONGSTRCPY(dst, src) \
do { \
	strncpy(dst, src, LONG_STRING); \
	dst[LONG_STRING - 1] = '\0'; \
} while (0)

#define SPRINTHUGESTR(dst, string, args...) \
do { \
	snprintf(dst, HUGE_STRING, string, args); \
	dst[HUGE_STRING - 1] = '\0'; \
} while (0)

#define VSPRINTHUGESTR(dst, string, list) \
do { \
	vsnprintf(dst, HUGE_STRING, string, list); \
	dst[HUGE_STRING - 1] = '\0'; \
} while (0)


#endif /* END OF HEADER FILE */
/******************************************************************************/

