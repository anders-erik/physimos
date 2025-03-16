
#pragma once

#include <string>


namespace xprubik {

/** Verifies the XP_ROOT_DIR. Returns false on failed verification. */
bool fs_init();
/** Reads text file relative to XP_ROOT_DIR.  */
std::string cat(std::string path_str);

}