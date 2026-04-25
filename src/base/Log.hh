#pragma once


// Std includes
#include <string>
#include <memory>
#include <sstream>
#include <string_view>
#include <format>
#include <vector>
#include <source_location>
// SpdLog includes
#include <spdlog/spdlog.h>


namespace TC {


// Enumerated values for spdlog levels
enum log_level_enum : int
{
    trace = SPDLOG_LEVEL_TRACE,
    debug = SPDLOG_LEVEL_DEBUG,
    info  = SPDLOG_LEVEL_INFO,
    warn  = SPDLOG_LEVEL_WARN,
    error = SPDLOG_LEVEL_ERROR,
    critical = SPDLOG_LEVEL_CRITICAL,
    off   = SPDLOG_LEVEL_OFF,
    n_levels
};

// Enumerated values for the debug levels
enum class DebugLevels : std::int16_t
{
  debug0 = 0,
  debug1, debug2, debug3, debug4, debug5, debug6, debug7, debug8, debug9,
  debugmax
};

// Inject debug levels to the enclosing namespace
using enum DebugLevels;


// Flag showing if debug logging enabled or not
constexpr inline bool DebugLogging = true;

// Currently allowed debug level, inclusive
extern DebugLevels AllowedDebugLevel; 

// Current log file if defined
extern std::string LogFileName;

// Currently used log sinks
extern std::vector<spdlog::sink_ptr> LogSinks;

// The type of Logger pointers
using LoggerPtr_t = std::shared_ptr<spdlog::logger>;

// Main (default) Logger, used application wide
extern LoggerPtr_t MainLogger;


// Name of the main logger, for access purposes
constexpr const char* MainLogName = "main";


// Convert string representation of a log level to its enum value
int StrToLogLevel(std::string_view lvlstr);

// Initialize Logging
void InitLogging(int lvl = info,
                 std::string_view logFile = {},
                 int dbgLevel = -1,
                 std::string_view logLevel = {});

// Terminate Logging
void TermLogging();


// Utility class to allow stream inserters on spdlog
struct slog : public std::stringstream
{
  spdlog::level::level_enum m_lvl;
  explicit slog(int lvl) : m_lvl(static_cast<spdlog::level::level_enum>(lvl)) {}

  ~slog() { const std::string& ss = this->str(); if(!ss.empty()) spdlog::log(m_lvl, "{}", ss); }
};


//
// Various Log() overrides
// 
inline void Log(std::string_view sv)
{ spdlog::info("{}", sv); }

inline void Log(int lvl, std::string_view sv)
{ spdlog::log(static_cast<spdlog::level::level_enum>(lvl), "{}", sv); }

inline slog Log() { return slog(info); }

inline slog Log(int lvl) { return slog(lvl); }

inline void Log(const std::exception& e)
{ spdlog::error("{}", e.what()); }

// https://en.cppreference.com/w/cpp/utility/format/format_to 
template <typename... Args >
inline void Log(std::string_view fmt, Args&&... args)
{ spdlog::info( std::vformat(fmt, std::make_format_args(args...)) ); }

template <typename... Args >
inline void Log(int lvl, std::string_view fmt, Args&&... args)
{ spdlog::log( static_cast<spdlog::level::level_enum>(lvl), std::vformat(fmt, std::make_format_args(args...)) ); }

template <typename... Args >
inline void Log(DebugLevels dbglvl, std::string_view fmt, Args&&... args)
{ if(DebugLogging && dbglvl <= AllowedDebugLevel) spdlog::log( spdlog::level::debug, std::vformat(fmt, std::make_format_args(args...)) ); }


// Log with the location data
inline void LogLoc(int lvl, std::string_view sv, const std::source_location& loc = std::source_location::current())
{
  spdlog::log(static_cast<spdlog::level::level_enum>(lvl),
    std::format("{}:{}:{} {}", loc.file_name(), loc.line(), loc.function_name(), sv));
}



// Get a logger specified by a name. If name not specified then get the main (default) logger
LoggerPtr_t GetLogger(const char* logger_name = nullptr);

// Set default logging level for the default logger
inline void SetLogLevel(log_level_enum lvl)
{ spdlog::set_level(static_cast<spdlog::level::level_enum>(lvl)); }


} // end namespace
