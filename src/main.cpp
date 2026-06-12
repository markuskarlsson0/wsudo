#include "cli/help.h"
#include "cli/parser.h"

int main(int argc, char* argv[]) {
    cli::Arguments arguments = cli::parse(argc, argv);

    if (arguments.help) {
        cli::help();
    }

    return 0;
}
