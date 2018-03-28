/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

#include "dlog.h"


/******************************************************************************/
#ifdef _WIN32
#define syslog(...)
#endif


/******************************************************************************/

/** file stream, where to print the log */
static FILE *dlog_file = NULL;

/** callback to be used for logging */
static void (*dlog_callback)(const char *file, const char *function, int line, const char *type, const char *message) = NULL;

/** add this string at start of log-line in error-level */
static char el_string[] = "ERROR";

/** add this string at start of log-line in warning-level */
static char wl_string[] = "WARNING";

/** add this string at start of log-line in info-level */
static char il_string[] = "INFO";

/** add this string at start of log-line in debug-level */
static char dl_string[] = "DEBUG";

/** whether to print to stderr or not */
int print_stderr = 1;

/** whether to print to log or not */
int print_enable = 1;

/** whether to print to syslog or not */
int print_syslog = 0;

/** whether to use colors or not */
int colors_enable = 1;


/******************************************************************************/
/**
 *	Initialize log-system.
 */
void DLog_init(char *file)
{
	/* Set log stream. */
	if (file) dlog_file = fopen(file, "a");
	print_stderr = 1;
	print_syslog = 0;
}


/******************************************************************************/
/**
 * Initialize log-system to use syslog.
 */
void DLog_init_syslog(char *ident)
{
	/* Set log stream. */
	dlog_file = NULL;
#ifdef _WIN32
	print_syslog = 0;
	print_stderr = 1;
#else
	print_syslog = 1;
	print_stderr = 0;
	openlog(ident, LOG_PID, LOG_USER);
#endif
}


/******************************************************************************/
/**
 * Initialize log-system to use user defined callback.
 */
void DLog_init_callback(void (*callback)(const char *file, const char *function, int line, const char *type, const char *message))
{
	/* Set log stream. */
	dlog_file = NULL;
	print_syslog = 0;
	print_stderr = 0;
	dlog_callback = callback;
}


/******************************************************************************/
/**
 * Quit log-system.
 */
void DLog_quit(void)
{
	/* Set log stream. */
	if (dlog_file) fclose(dlog_file);
#ifdef _WIN32
	if (print_syslog) closelog();
#endif
}


/******************************************************************************/
/**
 * Print string to LOG. Use like printf().
 */
void DLog(const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s", buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_DEFAULT, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_INFO, "%s", buf);
		if (dlog_callback) dlog_callback("?", "?", 0, "", buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with file and line information. Use like printf().
 */
void DLog_flf(char *file, int line, char *func, const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{	
		if (dlog_file) fprintf(dlog_file, "%s:%s():%d: %s\n", file, func, line, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_PURPLE, "%s:%s():%d:", file, func, line);
			LDC_PRINT(LDC_DEFAULT, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_INFO, "%s:%s():%d: %s\n", file, func, line, buf);
		if (dlog_callback) dlog_callback(file, func, line, "", buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with error-level. Use like printf().
 */
void DLog_e(const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s\n", el_string, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_WHITEB LDC_BRED, "%s:", el_string);
			LDC_PRINT(LDC_WHITE LDC_BRED, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_ERR, "%s:%s\n", el_string, buf);
		if (dlog_callback) dlog_callback("?", "?", 0, el_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with file and line information. Use like printf().
 */
void DLog_flfe(const char *file, int line, const char *func, const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s:%s():%d: %s\n", el_string, file, func, line, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_WHITEB LDC_BRED, "%s:", el_string);
			LDC_PRINT(LDC_DGRAY LDC_BRED, "%s:%s():%d:", file, func, line);
			LDC_PRINT(LDC_WHITE LDC_BRED, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_ERR, "%s:%s:%s():%d: %s\n", el_string, file, func, line, buf);
		if (dlog_callback) dlog_callback(file, func, line, el_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with warning-level. Use like printf().
 */
void DLog_w(const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s\n", wl_string, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_DGRAYB LDC_BYELLOW, "%s:", wl_string);
			LDC_PRINT(LDC_DGRAY LDC_BYELLOW, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_WARNING, "%s:%s\n", wl_string, buf);
		if (dlog_callback) dlog_callback("?", "?", 0, wl_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with file and line information. Use like printf().
 */
void DLog_flfw(const char *file, int line, const char *func, const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s:%s():%d: %s\n", wl_string, file, func, line, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_DGRAYB LDC_BYELLOW, "%s:", wl_string);
			LDC_PRINT(LDC_DGRAY LDC_BYELLOW, "%s:%s():%d:", file, func, line);
			LDC_PRINT(LDC_DGRAYB LDC_BYELLOW, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_WARNING, "%s:%s:%s():%d: %s\n", wl_string, file, func, line, buf);
		if (dlog_callback) dlog_callback(file, func, line, wl_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with info-level. Use like printf().
 */
void DLog_i(const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s\n", il_string, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_BLUE, "%s:", il_string);
			LDC_PRINT(LDC_DEFAULT, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_INFO, "%s:%s\n", il_string, buf);
		if (dlog_callback) dlog_callback("?", "?", 0, il_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with file and line information. Use like printf().
 */
void DLog_flfi(const char *file, int line, const char *func, const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s:%s():%d: %s\n", il_string, file, func, line, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_BLUE, "%s:", il_string);
			LDC_PRINT(LDC_PURPLE, "%s:%s():%d:", file, func, line);
			LDC_PRINT(LDC_DEFAULT, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_INFO, "%s:%s:%s():%d: %s\n", il_string, file, func, line, buf);
		if (dlog_callback) dlog_callback(file, func, line, il_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with debug-level. Use like printf().
 */
void DLog_d(const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s\n", dl_string, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_BLUE, "%s:", dl_string);
			LDC_PRINT(LDC_DEFAULT, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_INFO, "%s: %s\n", dl_string, buf);
		if (dlog_callback) dlog_callback("?", "?", 0, dl_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/**
 * Print string to LOG with file and line information. Use like printf().
 */
void DLog_flfd(const char *file, int line, const char *func, const char *string, ...)
{
	va_list args;
	char buf[512];

	/* Get args. */
	va_start(args, string);
	vsnprintf(buf, sizeof(buf), string, args);

	/* Print to log. */
	if (print_enable)
	{
		if (dlog_file) fprintf(dlog_file, "%s:%s:%s():%d: %s\n", dl_string, file, func, line, buf);
		if (print_stderr)
		{
			LDC_PRINT(LDC_CYANB LDC_BDGRAY, "%s:", dl_string);
			LDC_PRINT(LDC_PURPLE LDC_BDGRAY, "%s:%s():%d:", file, func, line);
			LDC_PRINT(LDC_CYAN LDC_BDGRAY, " %s", buf);
			LDC_PRINT(LDC_DEFAULT, "\n");
		}
		if (print_syslog) syslog(LOG_DEBUG, "%s:%s:%s():%d: %s\n", dl_string, file, func, line, buf);
		if (dlog_callback) dlog_callback(file, func, line, dl_string, buf);
	}

	/* End args. */
	va_end(args);
}


/******************************************************************************/
/** Flush log. */
void DLog_flush(void)
{
	if (dlog_file) fflush(dlog_file);
}


/******************************************************************************/
/** Enable printing to stderr. */
void DLog_enable_stderr(void)
{
	print_stderr = 1;
}


/******************************************************************************/
/** Disable printing to stderr. */
void DLog_disable_stderr(void)
{
	print_stderr = 0;
}


/******************************************************************************/
/** Enable printing to log. */
void DLog_enable(void)
{
	print_enable = 1;
}


/******************************************************************************/
/** Disable printing to log. */
void DLog_disable(void)
{
	print_enable = 0;
}


/******************************************************************************/
/** Enable colors. */
void DLog_enable_colors(void)
{
	colors_enable = 1;
}


/******************************************************************************/
/** Disable colors. */
void DLog_disable_colors(void)
{
	colors_enable = 0;
}


