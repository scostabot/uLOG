#include <ulog.h>

ULOG_DECL

int main(int argc, char **argv) {
        ULOG_ENABLE(ulog_output_stdout);
        ULOG("Name of program is <%s>", argv[0]);

        ULOG_FORMAT("%t %f:%l <%v>\n");
        ULOG("An event is occurred");
        ULOG_FORMAT("%v\n");
        ULOG("Trace output can be very terse also");
}

