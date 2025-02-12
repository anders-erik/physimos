#include <iostream>

#include "result.hh"


namespace plib {



void handleResult(void* resultInfoVoid) {
    plib::ResultInfo* resultInfo = resultInfo;

    

    std::cout << "resultInfo.error = " << resultInfo->error << std::endl;
    
}

}