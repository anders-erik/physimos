
#include "ptest.hh"

#include <iostream>

namespace ptest {


    // Log all passed asserts
    bool PTEST_VERBOSE_LOG = true;
    bool PTEST_EXIT_ON_FAILED_ASSERT = true;


    void passed(std::string msg) {
        std::cout << "pass  : " << msg << std::endl;

    }

    void failed(std::string msg) {
        std::cout << "_________________________________________________________________________________________" << std::endl;
        std::cout << "*" << std::endl;
        std::cout << "*     FAILED TEST:           " << std::endl;
        std::cout << "*" << std::endl;
        std::cout << "*     " << msg << std::endl;
        std::cout << "*" << std::endl;
        std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << std::endl;
    }

    // Primary ptest assert
    void assertTrue(bool expression, std::string message) {

        // on ok asserts we always return
        if (expression) {
            if (PTEST_VERBOSE_LOG)
                passed(message);
            return;
        }

        // Always log failed asserts
        failed(message);

        if (PTEST_EXIT_ON_FAILED_ASSERT) {
            std::cout << "Failed Assert with 'PTEST_EXIT_ON_FAILED_ASSERT = true'." << std::endl;
            std::cout << "Exiting." << std::endl;
            exit(0);
        }

        return;
    }


    // has to be run from within the physimos project!
    std::string getGitRepoRootDir() {
        char getRepoRootDirCommand[] = "git rev-parse --show-toplevel";

        // int returnCode = system(getRepoRootDirCommand);
        // std::cout << "returnCode = " << returnCode << std::endl;

        FILE* pipe = popen(getRepoRootDirCommand, "r");
        char buffer[128];
        if (pipe) {
            while (!feof(pipe)) {
                if (fgets(buffer, 128, pipe) != nullptr) {
                    // printf("%s", buffer);
                }
            }
            pclose(pipe);
        }

        std::string repoDir = std::string(buffer);
        // Clear trailing whitespace
        repoDir = repoDir.substr(0, repoDir.size() - 1);
        return repoDir;
    }

}