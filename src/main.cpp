#include "app/backend.h"
#include "app/frontend.h"
#include "cli/help.h"
#include "cli/parser.h"
#include "cli/version.h"
#include "utils/debug.h"
#include "utils/log.h"
#include <Windows.h>
#include <exception>

int wmain(int argc, wchar_t* argv[]) try {
    bool debug = utils::checkDebugFlag(argc, argv);

    if (debug) {
        utils::waitForDebugger();
    }

    cli::Arguments arguments = cli::parse(GetCommandLineW(), debug);

    if (arguments.help) {
        cli::help();
    } else if (arguments.version) {
        cli::version();
    } else if (arguments.backend) {
        app::backend(arguments.pipeName);
    } else {
        app::frontend(arguments.command, debug);
    }

    return 0;
} catch (const std::exception& exception) {
    utils::logException(exception);
    return 1;
}
