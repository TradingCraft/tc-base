
#include "Log.hh"
//-include <spdlog/spdlog.h>
#include <gtest/gtest.h>

using namespace TC;

TEST(LogTest, GetLoggerReturnsNonNull)
{
    EXPECT_NE(GetLogger(), nullptr);
}

TEST(LogTest, SetLogLevelTakesEffect)
{
    SetLogLevel(warn);
    EXPECT_EQ(spdlog::get_level(), spdlog::level::warn);
    SetLogLevel(info);
    EXPECT_EQ(spdlog::get_level(), spdlog::level::info);
}

TEST(LogTest, BasicLogDoesNotThrow)
{
    EXPECT_NO_THROW(Log("info string"));
    EXPECT_NO_THROW(Log(warn, "warn message"));
    EXPECT_NO_THROW(Log("{} {}", "formatted", 42));
    EXPECT_NO_THROW(Log(info, "{} {}", "formatted", 99));
}

TEST(LogTest, StreamOperatorDoesNotThrow)
{
    EXPECT_NO_THROW(Log() << "stream info");
    EXPECT_NO_THROW(Log(warn) << "stream warn " << 42);
}

TEST(LogTest, DebugLevelsDoNotThrow)
{
    EXPECT_NO_THROW(Log(debug1, "debug-1"));
    EXPECT_NO_THROW(Log(debug2, "debug-2 {}", 2));
}

TEST(LogTest, LogLocDoesNotThrow)
{
    EXPECT_NO_THROW(LogLoc(info, "with source location"));
}

TEST(LogTest, InitAndTermAreIdempotent)
{
    EXPECT_NO_THROW(TermLogging());
    EXPECT_NO_THROW(InitLogging());
    EXPECT_NO_THROW(InitLogging());
    EXPECT_NO_THROW(TermLogging());
    EXPECT_NO_THROW(TermLogging());
    EXPECT_NO_THROW(InitLogging());
}
