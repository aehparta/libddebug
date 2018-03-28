/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

/******************************************************************************/
/* INCLUDES */
#include "filechange.h"


/******************************************************************************/
/* FUNCTIONS */

/******************************************************************************/
/**
	Check, whether file has changed since last check.
	(Given modification timestamp is in the past compared to the one found
	from file)
	
	@param file Filename.
	@param ts Last time modification stamp.
	@return 0 if no change, 1 if changed, -1 on errors.
*/
int stat_file(char *file, time_t ts)
{
	/* Variables. */
	struct stat fsinfo;
	int err = 0;

	err = stat(file, &fsinfo);
	IF_ERR(err, -1, "Failed to stat file: \"%s\"!\n", file);

	if (ts < fsinfo.st_mtime) err = 1;

out_err:
	return (err);
}
/* END OF FUNCTION */



