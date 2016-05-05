#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <ulog.h>
#include "lib1.h"

int trace_flag=0;
int level_flag=0;

ULOG_DECL

void sig_handler(int signo)
{
	switch (signo) {
	case SIGUSR1:
		trace_flag = !trace_flag;
		if (trace_flag) {
			ulog_flags_t flags = { .timedentry = 1 };
			ULOG_FLAGS(flags);
			ULOG_FORMAT("%t %m %f:%l <%v>\n");
			ULOG_ENABLE(ulog_output_stderr);
		} else
			ULOG_DISABLE();
		break;
	case SIGUSR2:
		level_flag = !level_flag;
		if (level_flag)
			ULOG_LEVEL_SET(LIB1_ID);
		else
			ULOG_LEVEL_SET(ULOG_TRACE_MAIN);
		break;
	case SIGINT:
		ULOG_DISABLE();
		break;
	}
}

int main(short argc, char *argv[]) {
	int i = 0;

	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
		perror("can't catch SIGUSR1");

	if (signal(SIGUSR2, sig_handler) == SIG_ERR)
		perror("can't catch SIGUSR2");

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		perror("can't catch SIGINT");

	while(1) {
		sleep(4);
		ULOG("call lib function");
		lib1_call1(i++);
	}

	exit(0);
}
