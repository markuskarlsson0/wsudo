#include "app/backend.h"
#include "app/frontend.h"
#include "cli/help.h"
#include "cli/parser.h"
#include "cli/version.h"
#include "utils/log.h"
#include <exception>

int main(int argc, char* argv[]) try {
    cli::Arguments arguments = cli::parse(argc, argv);

    if (arguments.help) {
        cli::help();
    } else if (arguments.version) {
        cli::version();
    } else if (arguments.backend) {
        app::Backend backend(arguments.processId, arguments.command);
    } else {
        app::Frontend frontend(arguments.command);
    }

    return 0;
} catch (const std::exception& exception) {
    utils::logException(exception);
    return 1;
}
