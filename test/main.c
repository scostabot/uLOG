/**
 ** uLOG module test program.
 ** (c) mestesso.it 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "function1.h"
#include "lib1.h"

#ifdef ULOG_LIB
#include "uloglib.h"
#else
#include "ulog.h"
#endif

ULOG_DECL

int testn = 'A'; // By default run all tests

#define TEST(a) ((a == testn) || ('A' == testn) ? 1 : 0)

void test_previous(void)
{
	ULOG_ENABLE(ulog_output_previous);
	ULOG("Re-enable previous setting");
	ULOG_DISABLE();
}

char *arrayStrings[] = {
	"First",
	"Second",
	"Third",
	"",
	"Fourth",
	"Fifth",
	NULL
};

char arrayBytes[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xBB, 0xE0 };
short arrayShorts[] = { 0x12DE, 0x34AD, 0x45BE, 0x67EF, 0x89BA, 0xBB, 0xE0 };

int main(int argc, char **argv) {
#ifndef ULOG_REMOVE
	char *mmmh = "first";
#endif
	int rc = 0;
#ifndef ULOG_REMOVE
	int hex = 0x88;
#endif

	if (argc > 1)
		testn = atoi(argv[1]);

	ULOG_ENABLE(ulog_output_stdout);
	ULOG("We are testing uLOG version %s", ULOG_VERSION_STR);

	if(TEST(1)) {
		ULOG_ENABLE(ulog_output_stderr);
		ULOG("This goes to standard error");
		ULOG("Before disable logging...");
		ULOG_DISABLE();
		ULOG("IF YOU SEE THIS IT'S AN ERROR!!!");
		test_previous();
	}

	if(TEST(2)) {
		ULOG_ENABLE(ulog_output_stdout);
		ULOG("This goes to standard output");
		ULOG("Before disable logging...");
		ULOG_DISABLE();
		ULOG("IF YOU SEE THIS IT'S AN ERROR!!!");
		test_previous();
	}

	if(TEST(3)) {
		ULOG_FILE("/tmp/trace");
		ULOG_ENABLE(ulog_output_file);
		ULOG("Goes into a file....");
		ULOG("Before disable logging...");
		ULOG_DISABLE();
		ULOG("IF YOU SEE THIS IT'S AN ERROR!!!");
		test_previous();
	}

	// From now on we keep standard output for tracing
	ULOG_ENABLE(ulog_output_stdout);

	if(TEST(4))
		ULOG("Variadic support first=%s, second (136)=%dd, third (0x88)=0x%X", mmmh, hex, hex);

	if(TEST(5))
		ULOG("Source file and line number displayed file=main.c line=96");

	if(TEST(6))
		function1();

	if(TEST(7)) {
		ULOG("Change output format from %s to %%t %%f:%%l <%v>\\n", ULOG_FORMAT_DEFAULT_PRT);
		ULOG_FORMAT("%t %f:%l <%v>\n");
		ULOG("timestamp file:line");
		ULOG_FORMAT(ULOG_FORMAT_DEFAULT);
		ULOG("back to default format");
	}

	if(TEST(8)) {
		ULOGA(arrayStrings, 0, 0);
		ULOG_AFORMAT("execvp(", "\"", "%s", "\" ", ");");
		ULOGA(arrayStrings, 0, 0);
		ULOG_AFORMAT("unsigned char bytes[", "", "0x%.2X", ", " , "];");
		ULOGA(arrayBytes, sizeof(char), sizeof(arrayBytes)/sizeof(char));
		ULOG_AFORMAT("unsigned short shorts[", "", "0x%.2X", ", " , "];");
		ULOGA(arrayShorts, sizeof(short), sizeof(arrayShorts)/sizeof(short));
	}

	if(TEST(9)) {
		ulog_flags_t flags = { .timedentry = 1 };
		ULOG_FLAGS(flags);
		ULOG_DISABLE();
		ULOG_ENABLE(ulog_output_stdout);
	}

	/* See lib1.c for details of tests */

	if(TEST(10)) {
		lib1_call1_simplest();
	}

	if(TEST(11)) {
		lib1_call2_slightly_more_complicated();
	}

	ULOG("End of test(s)");

	exit(rc);
}
