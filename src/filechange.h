/*
 * DDebuglib
 *
 * License: MIT, see COPYING
 * Authors: Antti Partanen <aehparta@cc.hut.fi, duge at IRCnet>
 */

#ifndef FILECHANGE_H
#define FILECHANGE_H


/******************************************************************************/
/* INCLUDES */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "ddebuglib.h"


/******************************************************************************/
/* FUNCTION DEFINITIONS */
int stat_file(char *, time_t);


#endif /* END OF HEADER FILE */
/******************************************************************************/

