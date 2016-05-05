
#include "function1.h"
#ifdef ULOG_LIB
#include "uloglib.h"
#else
#include "ulog.h"
#endif

int what;

void function1(void)
{
	ULOG("Inside function1...");
	what = 5;
	ULOG("...before exiting");
}
