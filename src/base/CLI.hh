#pragma once


// Std
#include <string>
#include <string_view>
// Prj
#include <CLI/CLI.hpp>



namespace TC { 

  // Globals to store
  extern std::string CliLogFile;
  extern std::string CliLogLevel;
  extern int CliDbgLevel;

  // Get default Cli Log File Name
  std::string DefaultCliLogFileName(std::string_view appName);

  // Configure CLI options for a given application
  int ConfigureCLI(CLI::App& app, int argc, char* const argv[]);

}
