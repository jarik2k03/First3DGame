#pragma once
#include <string>
#include <sstream>
#include <debugapi.h>

#define CONSOLEDEBUG(ss) \
    OutputDebugStringA(ss.str().c_str());

using stlstr = std::string;
using stlcstr = const std::string;
using stlwstr = std::wstring;
using stlcwstr = const std::wstring;
using sstream = std::stringstream;