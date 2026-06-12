#include "cli/help.h"
#include "cli/parser.h"
#include "cli/version.h"

int main(int argc, char* argv[]) {
    cli::Arguments arguments = cli::parse(argc, argv);

    if (arguments.help) {
        cli::help();
    } else if (arguments.version) {
        cli::version();
    }

    return 0;
}
