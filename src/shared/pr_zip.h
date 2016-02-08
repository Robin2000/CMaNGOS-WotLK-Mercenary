
#ifndef PR_ZIP_H
#define PR_ZIP_H

#include "zlib/zlib.h"
std::string MANGOS_DLL_SPEC pr_compress(const std::string& str, int compressionlevel = Z_BEST_SPEED);
std::string MANGOS_DLL_SPEC pr_decompress(const std::string& str);
#endif
