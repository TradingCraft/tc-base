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

  // Configure CLI options for a given application.
  // Returns: -1 = continue running, 0 = clean exit (--help/--version), >0 = parse error exit code
  // Typical call site:
  //   int rc = TC::ConfigureCLI(app, argc, argv);
  //   if (rc >= 0) return rc;  // 0 = help/version printed, >0 = parse error
  int ConfigureCLI(CLI::App& app, int argc, char* const argv[]);

}
