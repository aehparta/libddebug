/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

#ifndef LOG_H
#define LOG_H

/******************************************************************************/
#ifdef _WIN32
#define DLLEXP __declspec(dllexport)
#else
#define DLLEXP
#endif


/******************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>


/******************************************************************************/

#ifndef _DEBUGLIB_REPLACE_DEFINITIONS

/** Macro definition for DLogflf to ease "__LINE__, __FILE__,__FUNCTION__" print. */
#define _FLF				__FILE__,__LINE__,__FUNCTION__

/** Macro definition. */
#define IF_ER(errval, retval) \
do { \
	if ((errval) != 0) { \
		err = retval; \
		goto out_err; \
	} \
} while (0)

/** Macro definition. */
#define OUT(retval) do { err = retval; goto out_err; } while (0)

/** Macro definition. */
#ifdef _DEBUG
#define IF_ERR(errval, retval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_flfe(_FLF, args); \
		err = retval; \
		goto out_err; \
	} \
} while (0)
#define DEBUG_MSG(...) DLog_flfd(_FLF, __VA_ARGS__);
#define INFO_MSG(...) DLog_flfi(_FLF, __VA_ARGS__);
#define ERROR_MSG(...) DLog_flfe(_FLF, __VA_ARGS__);
#define WARNING_MSG(...) DLog_flfw(_FLF, __VA_ARGS__);
#define IF_EMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_flfe(_FLF, args); \
	} \
} while (0)
#define IF_IMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_flfi(_FLF, args); \
	} \
} while (0)
#define IF_WMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_flfw(_FLF, args); \
	} \
} while (0)
#define IF_DMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_flfd(_FLF, args); \
	} \
} while (0)

#else
#define IF_ERR(errval, retval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_e(args); \
		err = retval; \
		goto out_err; \
	} \
} while (0)
#define DEBUG_MSG(...)
#define INFO_MSG(...) DLog_i(__VA_ARGS__);
#define ERROR_MSG(...) DLog_e(__VA_ARGS__);
#define WARNING_MSG(...) DLog_w(__VA_ARGS__);
#define IF_EMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_e(args); \
	} \
} while (0)
#define IF_IMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_i(args); \
	} \
} while (0)
#define IF_WMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_w(args); \
	} \
} while (0)
#define IF_DMSG(errval, args...) \
do { \
	if ((errval) != 0) { \
		DLog_d(args); \
	} \
} while (0)
#endif

#define _DEBUG_MSG(...)
#define _INFO_MSG(...)
#define _ERROR_MSG(...)
#define _WARNING_MSG(...)

#define LDC_DEFAULT "\033[0m"
#define LDC_DGRAYB "\033[1;30m"
#define LDC_REDB "\033[1;31m"
#define LDC_GREENB "\033[1;32m"
#define LDC_YELLOWB "\033[1;33m"
#define LDC_BLUEB "\033[1;34m"
#define LDC_PURPLEB "\033[1;35m"
#define LDC_CYANB "\033[1;36m"
#define LDC_WHITEB "\033[1;37m"

#define LDC_DGRAY "\033[30m"
#define LDC_RED "\033[31m"
#define LDC_GREEN "\033[32m"
#define LDC_YELLOW "\033[33m"
#define LDC_BLUE "\033[34m"
#define LDC_PURPLE "\033[35m"
#define LDC_CYAN "\033[36m"
#define LDC_WHITE "\033[37m"

#define LDC_BDGRAY "\033[40m"
#define LDC_BRED "\033[41m"
#define LDC_BGREEN "\033[42m"
#define LDC_BYELLOW "\033[43m"
#define LDC_BBLUE "\033[44m"
#define LDC_BPURPLE "\033[45m"
#define LDC_BCYAN "\033[46m"
#define LDC_BWHITE "\033[47m"

#define LDC_SET(color) \
do { if (colors_enable) fprintf(stderr, color); } while(0)

#define LDC_PRINT(color, args...) \
do { \
if (colors_enable) fprintf(stderr, color); \
fprintf(stderr, args); \
if (colors_enable) fprintf(stderr, LDC_DEFAULT); \
} while(0)


#endif


/******************************************************************************/
/* FUNCTION DEFINITIONS */
void DLLEXP DLog_init(char *);
void DLLEXP DLog_init_syslog(char *);
void DLLEXP DLog_quit(void);
void DLLEXP DLog_init_callback(void (*callback)(const char *file, const char *function, int line, const char *type, const char *message));

void DLLEXP DLog(const char *string, ...);
void DLLEXP DLog_flf(char *, int, char *, const char *, ...);

void DLLEXP DLog_e(const char *string, ...);
void DLLEXP DLog_flfe(const char *, int, const char *, const char *, ...);
void DLLEXP DLog_w(const char *string, ...);
void DLLEXP DLog_flfw(const char *, int, const char *, const char *, ...);
void DLLEXP DLog_i(const char *string, ...);
void DLLEXP DLog_flfi(const char *, int, const char *, const char *, ...);

void DLLEXP DLog_d(const char *, ...);
void DLLEXP DLog_flfd(const char *, int, const char *, const char *, ...);

void DLLEXP DLog_flush(void);

void DLLEXP DLog_enable_stderr(void);
void DLLEXP DLog_disable_stderr(void);
void DLLEXP DLog_enable(void);
void DLLEXP DLog_disable(void);
void DLLEXP DLog_enable_colors(void);
void DLLEXP DLog_disable_colors(void);


#endif /* END OF HEADER FILE */
/******************************************************************************/

