#include <stdio.h>

#include "html.h"
#include "lib/parser/parser.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: brd_cli filename.brd\n");
        exit(1);
    }

    std::string fname = argv[1];
    auto parsed_file = parse_file(argv[1]);
    if (parsed_file.has_value()) {
        // stream_file(fname, *parsed_file);
    }
}
