#include "CLI.hh"
#include "Log.hh"
#include <gtest/gtest.h>


using namespace std;
using namespace TC;

// The main entry point for all test cases.
int main(int argc, char *argv[]) 
{
  int rc{};

  // First, pass arguments to Gtest
  // it will catch args such as --gtest_filter=Log_test.Base
  ::testing::InitGoogleTest(&argc, argv);

  // Then pass arguments to CLI
  CLI::App app;  
  rc = ConfigureCLI(app, argc, argv);
  if(rc) return rc;

  InitLogging();  // Initialize the logging 
  Log(info,"Starting {}", argv[0]);

  rc = RUN_ALL_TESTS();
  TermLogging();
  return rc;
}


