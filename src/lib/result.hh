#ifndef RESULT_HH
#define RESULT_HH

namespace plib {


typedef enum ResultStatus {
    Error = 0,
    Ok = 1,
} ResultStatus;


typedef enum ResultErrors {
    ErrorOpenFile = 10,
    ErrorFileFormat = 11,

    ErrorParsing = 12,

    ErrorUnknown = 13,

    // Physimos specific
    RootDirNotFound = 100,
} ResultErrors;


/// Provides Result information on ResultStatus = Error
typedef struct ResultInfo {
    ResultErrors error;
    std::string info;
} ResultInfo;


/// Provides additional context to returned pointers that is likely to occationally be null.
/// Primary purpose is to enables improved reasoning and handling of errors.
/// 
/// On Error the pointer should reference a ResultInfo-object instead of a nullptr. Thus no nullptrs should be accessible by caller.
typedef struct Result {
    ResultStatus status = ResultStatus::Error;
    void * pointer = nullptr;
} Result;


void handleResult(void* resultInfoVoid);

}



#endif