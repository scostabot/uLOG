/**
 ** uLOG module test program, uLOG trace module.
 ** (c) mestesso.it 2015
 */

#include "lib1.h"

#include "ulog.h"

#define LIB1_ID 2

ulog_module_t seclib = { "LIB1", LIB1_ID };

ULOG_MODULE_DECL(&seclib);  /* This map the current file <--> trace module.
                               You can associate multiple files with the same
                               trace module so you can logically split things
                               as you wish.                                   */

static unsigned int local;

void lib1_init(void)
{
	/* Define a new trace module. This is performed
	   once in the life of the program.             */

	ULOG_MODULE_ADD(&seclib);

	local = 0xdeadbeef;
}

void lib1_call1_simplest(void)
{
	/* Simplest use of tracemodule is hardcoded stuff,
	   i.e., use directly hardcoded bitmaps to enable
	   or disable desired module. We do this in this
	   function.                                      */

	ULOG_LEVEL_SET(ULOG_TRACE_ALL);
	ULOG_FORMAT("%t %m %f:%l <%v>\n");
	ULOG("timestamp module-name (%s) file(lib1.c):line(%d)", seclib.name, __LINE__);
	ULOG_LEVEL_SET(ULOG_TRACE_NONE);
	ULOG("IF YOU SEE THIS IT'S AN ERROR!!!");
	ULOG_LEVEL_SET(LIB1_ID);
	ULOG_FORMAT("%m %f:%l <%v>\n");
	ULOG("module-name (%s) file(lib1.c):line(%d) enabled only (so you won't see End of test(s))", seclib.name, __LINE__);
}

void lib1_call2_slightly_more_complicated(void)
{
	/* In all those case where you cannot determine compile-time
	   your module id (or you don't want to) it is possible to
	   let the library allocate run-time an id for you.
	   Define a new trace module. This is performed
	   once in the life of the program.                           */

	ULOG_LEVEL_SET(ULOG_TRACE_ALL);

	ULOG_MODULE_ADD(&seclib);
}
