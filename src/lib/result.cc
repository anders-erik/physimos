#include <iostream>

#include "result.hh"


namespace plib {



void handleResult(void* resultInfoVoid) {
    plib::ResultInfo* resultInfo = (plib::ResultInfo*)resultInfoVoid;

    std::cout << "resultInfo.>error = " << resultInfo->error << std::endl;
    std::cout << "resultInfo.>info  = " << resultInfo->info << std::endl;
    
}

}