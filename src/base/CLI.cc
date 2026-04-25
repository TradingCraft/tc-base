/** \file CLI.cc
 * Definitions for CLI Command Line Flags support.
 *
 *
 *
 */

#include "CLI.hh"
#include "version.hh"
#include <filesystem>

namespace fs = std::filesystem;

namespace TC {

  
  // Globals
  std::string CliLogFile{};
  std::string CliLogLevel;
  int CliDbgLevel{-1};



  std::string DefaultCliLogFileName(std::string_view appName)
  {
    fs::path ap = appName;
    ap.replace_extension(".log");
    return ap.string();
  }
  

  int ConfigureCLI(CLI::App& app, int argc, char* const argv[])
  {
    auto verStr = std::string("v") + TC_VERSION
                + " (" + TC_BUILD_TYPE + ")"
                + " [" + TC_GIT_HASH + "]"
                + " built with " + TC_CXX_COMPILER_ID + " " + TC_CXX_COMPILER_VERSION;
    app.set_version_flag("-V,--version", verStr);

    app.add_option("--log-file", CliLogFile, "Log file")
      ->take_last();

    app.add_option("-d,--debug", CliDbgLevel, "Debug level (0-9) 0=off 9=highest")
      ->take_last()->check(CLI::Range(0, 9))->envname("DBGLVL");

    app.add_option("-l,--log-level", CliLogLevel, "Set log level (error, warn, info, debug, trace)")
      ->take_last();

    try {
      app.footer(verStr);
      app.parse(argc, argv);
    }
    catch (const CLI::ParseError& e) {
      return app.exit(e); // 0 for --help/--version, >0 for parse errors
    }

    return -1; // Parsing succeeded; caller should continue
  }


} // namespace




