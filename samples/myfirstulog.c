#include <ulog.h>

ULOG_DECL

int main(int argc, char **argv) {
        ULOG_ENABLE(ulog_output_stdout);
        ULOG("Name of program is %s", argv[0]);
}

