#include <ulog.h>

ULOG_DECL


int main(int argc, char **argv) {
        ULOG_ENABLE(ulog_output_stdout);
        ULOG("Name of program is <%s>", argv[0]);

        ULOG("Before disable logging...");
        ULOG_DISABLE();
        ULOG("YOU DON'T SEE IT!!");

        ULOG_FILE("/tmp/trace");
        ULOG_FLAGS(ULOG_TIMEDENTRY);

        ULOG_ENABLE(ulog_output_file);
        ULOG("Goes into a file....");
}

