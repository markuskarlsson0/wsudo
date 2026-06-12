#include "cli/parser.h"

int main(int argc, char* argv[]) {
    cli::Arguments arguments = cli::parse(argc, argv);
    return 0;
}
