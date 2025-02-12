#ifndef PTEST_HH
#define PTEST_HH

#include <string>

namespace ptest {

// Log all passed asserts
extern bool PTEST_VERBOSE_LOG;
extern bool PTEST_EXIT_ON_FAILED_ASSERT;

void passed(std::string msg);
void failed(std::string msg);
void assertTrue(bool expression, std::string message);

std::string getGitRepoRootDir();

}

#endif