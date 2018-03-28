/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

/******************************************************************************/
/* INCLUDES */
#include "system.h"


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
 * Execute new application.
 *
 * @param exe Executable name.
 * @return Return PID on success, -1 on errors.
 */
int exec_application(char *exe, ...)
{
	va_list args;
	int err = 0;

	if (strlen(exe) > 0) err = fork();
	else err = -1;

	if (err < 0);
	else if (err > 0);
	else if(err == 0)
	{
		va_start(args, exe);
		err = execvp(exe, (char **)args);
		if (err != 0)
		{	
			perror("exec failed");
			exit(1);
		}
	}

out_err:
	return err;
}

