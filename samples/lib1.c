/**
 ** uLOG module test program, uLOG trace module.
 ** (c) mestesso.it 2015
 */

#include <ulog.h>

#include "lib1.h"

ulog_module_t seclib = { "LIB1", LIB1_ID };

ULOG_MODULE_DECL(&seclib);  /* This map the current file <--> trace module.
                               You can associate multiple files with the same
                               trace module so you can logically split things
                               as you wish.                                   */

void lib1_call1(int input)
{
	/* Simplest use of tracemodule is hardcoded stuff,
	   i.e., use directly hardcoded bitmaps to enable
	   or disable desired module. We do this in this
	   function.                                      */

	ULOG("lib1_call1(%d)", input);
}
