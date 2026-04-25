
#include "Log.hh"
// Std includes
#include <stdexcept>
#include <vector>
// SpdLog includes
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace TC {

// Currently allowed debug level, inclusive
DebugLevels AllowedDebugLevel{ DebugLevels::debugmax };

// Current log file if defined
std::string LogFileName;

// Currently used Log Sinks
std::vector<spdlog::sink_ptr> LogSinks;

// Main (default) Logger, used application wide
LoggerPtr_t MainLogger;



int StrToLogLevel(std::string_view lvlstr)
{
    if(lvlstr == "trace")    return trace;
    if(lvlstr == "debug")    return debug;
    if(lvlstr == "info")     return info;
    if(lvlstr == "warn")     return warn;
    if(lvlstr == "error")    return error;
    if(lvlstr == "critical") return critical;
    if(lvlstr == "off")      return off;
    return info;
}


void InitLogging(int lvl, std::string_view logFile, int dbgLevel, std::string_view logLevel)
{
    if (MainLogger) {
        TermLogging();
    }

    LogSinks.clear();

    if(!logFile.empty()) LogFileName = logFile;
    if(dbgLevel >= 0) {
        AllowedDebugLevel = static_cast<DebugLevels>(dbgLevel);
        lvl = (dbgLevel > 0) ? debug : info;
    }
    if(!logLevel.empty()) {
        lvl = StrToLogLevel(logLevel);
        spdlog::info("Setting Log level to {} -> {}", logLevel, lvl);
        spdlog::set_level(static_cast<spdlog::level::level_enum>(lvl));
    }

    // Logging to multiple sinks
    // https://github.com/gabime/spdlog/wiki/2.-Creating-loggers
    spdlog::sink_ptr conSink = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
    //? conSink->set_level( static_cast<spdlog::level::level_enum>(lvl) );
    conSink->set_level( spdlog::level::trace ); // Make it highest level
    conSink->set_pattern("%C%m%d %H%M%S [%^%l%$] %v");
    LogSinks.push_back(conSink);
    

    if(!LogFileName.empty()) {
        spdlog::sink_ptr fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(LogFileName, 10*1024*1024, 10);
        //? fileSink->set_level( static_cast<spdlog::level::level_enum>(lvl) );
        fileSink->set_level( spdlog::level::trace ); // Make it highest level
        //? fileSink->set_pattern("%C%m%d %H%M%S.%e[%n-%P-%t-%l] %v");
        fileSink->set_pattern("%C%m%d %H%M%S.%e[%t-%l] %v"); 
        LogSinks.push_back(fileSink); 
    }


    MainLogger = std::make_shared<spdlog::logger>(MainLogName, begin(LogSinks), end(LogSinks));
    MainLogger->set_level( static_cast<spdlog::level::level_enum>(lvl) );
    // Register the combined logger to access it globally
    spdlog::register_logger(MainLogger);
    // Replace the default logger:
    // https://github.com/gabime/spdlog/wiki/Default-logger
    spdlog::set_default_logger(MainLogger);
}


void TermLogging()
{
    if (!MainLogger) {
        LogSinks.clear();
        spdlog::drop(MainLogName);
        return;
    }

    MainLogger->flush();
    spdlog::set_default_logger(nullptr);
    LogSinks.clear();
    MainLogger.reset();
    spdlog::drop(MainLogName);
    //? spdlog::drop_all();
}


// Get a logger specified by a name. If name not specfified then get the main logger
LoggerPtr_t GetLogger(const char* logger_name)
{
    auto logger = spdlog::get(std::string(logger_name ? logger_name : MainLogName));
    if(!logger)
        throw std::runtime_error("Spdlog not initialized.");

    return logger;
}


} // end namespace
