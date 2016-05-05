#include <ulog.h>

ULOG_DECL

char *arrayStrings[] = {
        "First", "Second", "Third", "", "Fourth", "Fifth", NULL
};
char arrayBytes[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xBA, 0xBB, 0xE0 };

int main(int argc, char **argv) {
        ULOG_ENABLE(ulog_output_stdout);

        ULOGA(arrayStrings, 0, 0);
        ULOG_AFORMAT("char arrayBytes{", "", "0x%.2X", ", " , "};");
        ULOGA(arrayBytes, sizeof(char), sizeof(arrayBytes)/sizeof(char));
}

