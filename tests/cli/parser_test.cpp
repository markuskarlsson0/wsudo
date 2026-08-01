#include "cli/parser.h"
#include <gtest/gtest.h>

namespace {

void expectCommand(const cli::Arguments& arguments, const std::wstring& command) {
    EXPECT_EQ(arguments.command, command);
    EXPECT_TRUE(arguments.pipeName.empty());
    EXPECT_FALSE(arguments.backend);
    EXPECT_FALSE(arguments.help);
    EXPECT_FALSE(arguments.version);
}

void expectBackend(const cli::Arguments& arguments, const std::wstring& pipeName) {
    EXPECT_TRUE(arguments.backend);
    EXPECT_EQ(arguments.pipeName, pipeName);
    EXPECT_TRUE(arguments.command.empty());
    EXPECT_FALSE(arguments.help);
    EXPECT_FALSE(arguments.version);
}

void expectHelp(const cli::Arguments& arguments) {
    EXPECT_TRUE(arguments.help);
    EXPECT_TRUE(arguments.command.empty());
    EXPECT_TRUE(arguments.pipeName.empty());
    EXPECT_FALSE(arguments.backend);
    EXPECT_FALSE(arguments.version);
}

void expectVersion(const cli::Arguments& arguments) {
    EXPECT_TRUE(arguments.version);
    EXPECT_TRUE(arguments.command.empty());
    EXPECT_TRUE(arguments.pipeName.empty());
    EXPECT_FALSE(arguments.backend);
    EXPECT_FALSE(arguments.help);
}

} // namespace

TEST(ParserTest, ParsesBackendPipeName) {
    std::wstring pipeName = LR"(\\.\pipe\wsudo\0123456789abcdef)";

    expectBackend(cli::parse(L"wsudo " + pipeName), pipeName);
}

TEST(ParserTest, RejectsEmptyBackendPipeName) {
    EXPECT_THROW(cli::parse(LR"(wsudo \\.\pipe\wsudo\)"), std::runtime_error);
}

TEST(ParserTest, ParsesNoArgumentsAsEmptyCommand) { expectCommand(cli::parse(L"wsudo"), L""); }

TEST(ParserTest, ParsesSingleWordCommand) {
    expectCommand(cli::parse(L"wsudo notepad.exe"), L"notepad.exe");
}

TEST(ParserTest, ParsesMultiWordCommand) {
    expectCommand(cli::parse(L"wsudo echo test 123"), L"echo test 123");
}

TEST(ParserTest, KeepsQuotingOfCommand) {
    expectCommand(cli::parse(LR"(wsudo echo "hello world")"), LR"(echo "hello world")");
}

TEST(ParserTest, SkipsQuotedExecutablePathContainingSpaces) {
    expectCommand(cli::parse(LR"("C:\my dir\wsudo.exe" echo "hello world")"),
                  LR"(echo "hello world")");
}

TEST(ParserTest, SkipsAllWhitespaceBeforeFirstArgument) {
    expectCommand(cli::parse(L"wsudo   \t echo hello"), L"echo hello");
}

TEST(ParserTest, SkipsDebugFlagWhenItWasConsumed) {
    expectCommand(cli::parse(LR"(wsudo --debug echo "hello world")", true),
                  LR"(echo "hello world")");
}

TEST(ParserTest, KeepsDebugFlagWhenItWasNotConsumed) {
    expectCommand(cli::parse(LR"(wsudo --debug echo "hello world")"),
                  LR"(--debug echo "hello world")");
}

TEST(ParserTest, ParsesShortHelpFlag) { expectHelp(cli::parse(L"wsudo -h")); }

TEST(ParserTest, ParsesLongHelpFlag) { expectHelp(cli::parse(L"wsudo --help")); }

TEST(ParserTest, ParsesShortVersionFlag) { expectVersion(cli::parse(L"wsudo -v")); }

TEST(ParserTest, ParsesLongVersionFlag) { expectVersion(cli::parse(L"wsudo --version")); }

TEST(ParserTest, IgnoresExtraArgumentsAfterHelpFlag) {
    expectHelp(cli::parse(L"wsudo -h ignored"));
}

TEST(ParserTest, IgnoresExtraArgumentsAfterVersionFlag) {
    expectVersion(cli::parse(L"wsudo -v ignored"));
}
