/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@iki.fi, duge at IRCnet>
 */

/******************************************************************************/
/* INCLUDES */
#include <stdio.h>
#include "debuglib.h"


/******************************************************************************/
/* simple example */
int main(int argc, char *argv[])
{
	int err = 0;
	DLog_init(NULL);

	INFO_MSG("example is now starting!");
	WARNING_MSG("btw all text goes now into stderr");
	ERROR_MSG("this is an error! (%d arguments given)", argc - 1);
	DEBUG_MSG("hej, we got this far!");

	IF_ERR(argc == 1, 1, "you didn't give any arguments!");

out_err:
	IF_IMSG(argc > 1, "you gave %d arguments", argc - 1);
	IF_EMSG(err == 1, "no arguments :(");

	return err;
}


