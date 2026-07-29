#include "cli/parser.h"
#include <gtest/gtest.h>

TEST(ParserTest, ParsesBackendPipeName) {
    wchar_t executable[] = L"wsudo";
    wchar_t pipeName[] = LR"(\\.\pipe\wsudo\0123456789abcdef)";
    wchar_t* argv[] = {executable, pipeName};

    cli::Arguments arguments = cli::parse(2, argv);

    EXPECT_TRUE(arguments.backend);
    EXPECT_EQ(arguments.pipeName, pipeName);
    EXPECT_TRUE(arguments.command.empty());
    EXPECT_FALSE(arguments.help);
    EXPECT_FALSE(arguments.version);
}

TEST(ParserTest, ParsesNoArgumentsAsEmptyCommand) {
    wchar_t executable[] = L"wsudo";
    wchar_t* argv[] = {executable};

    cli::Arguments arguments = cli::parse(1, argv);

    EXPECT_TRUE(arguments.command.empty());
    EXPECT_FALSE(arguments.backend);
    EXPECT_FALSE(arguments.help);
    EXPECT_FALSE(arguments.version);
}

TEST(ParserTest, ParsesSingleWordCommand) {
    wchar_t executable[] = L"wsudo";
    wchar_t command[] = L"notepad.exe";
    wchar_t* argv[] = {executable, command};

    cli::Arguments arguments = cli::parse(2, argv);

    EXPECT_EQ(arguments.command, L"notepad.exe");
    EXPECT_FALSE(arguments.backend);
    EXPECT_FALSE(arguments.help);
    EXPECT_FALSE(arguments.version);
}

TEST(ParserTest, ParsesMultiWordCommand) {
    wchar_t executable[] = L"wsudo";
    wchar_t part1[] = L"cmd.exe";
    wchar_t part2[] = L"/c";
    wchar_t part3[] = L"echo";
    wchar_t part4[] = L"test";
    wchar_t* argv[] = {executable, part1, part2, part3, part4};

    cli::Arguments arguments = cli::parse(5, argv);

    EXPECT_EQ(arguments.command, L"cmd.exe /c echo test");
    EXPECT_FALSE(arguments.backend);
}

TEST(ParserTest, ParsesShortHelpFlag) {
    wchar_t executable[] = L"wsudo";
    wchar_t flag[] = L"-h";
    wchar_t* argv[] = {executable, flag};

    cli::Arguments arguments = cli::parse(2, argv);

    EXPECT_TRUE(arguments.help);
    EXPECT_FALSE(arguments.version);
    EXPECT_FALSE(arguments.backend);
    EXPECT_TRUE(arguments.command.empty());
}

TEST(ParserTest, ParsesLongHelpFlag) {
    wchar_t executable[] = L"wsudo";
    wchar_t flag[] = L"--help";
    wchar_t* argv[] = {executable, flag};

    cli::Arguments arguments = cli::parse(2, argv);

    EXPECT_TRUE(arguments.help);
    EXPECT_FALSE(arguments.version);
}

TEST(ParserTest, ParsesShortVersionFlag) {
    wchar_t executable[] = L"wsudo";
    wchar_t flag[] = L"-v";
    wchar_t* argv[] = {executable, flag};

    cli::Arguments arguments = cli::parse(2, argv);

    EXPECT_TRUE(arguments.version);
    EXPECT_FALSE(arguments.help);
    EXPECT_FALSE(arguments.backend);
    EXPECT_TRUE(arguments.command.empty());
}

TEST(ParserTest, ParsesLongVersionFlag) {
    wchar_t executable[] = L"wsudo";
    wchar_t flag[] = L"--version";
    wchar_t* argv[] = {executable, flag};

    cli::Arguments arguments = cli::parse(2, argv);

    EXPECT_TRUE(arguments.version);
    EXPECT_FALSE(arguments.help);
}

TEST(ParserTest, IgnoresExtraArgumentsAfterHelpFlag) {
    wchar_t executable[] = L"wsudo";
    wchar_t flag[] = L"-h";
    wchar_t extra[] = L"ignored";
    wchar_t* argv[] = {executable, flag, extra};

    cli::Arguments arguments = cli::parse(3, argv);

    EXPECT_TRUE(arguments.help);
    EXPECT_TRUE(arguments.command.empty());
}

TEST(ParserTest, IgnoresExtraArgumentsAfterVersionFlag) {
    wchar_t executable[] = L"wsudo";
    wchar_t flag[] = L"-v";
    wchar_t extra[] = L"ignored";
    wchar_t* argv[] = {executable, flag, extra};

    cli::Arguments arguments = cli::parse(3, argv);

    EXPECT_TRUE(arguments.version);
    EXPECT_TRUE(arguments.command.empty());
}
