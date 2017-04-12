// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

//#define DEBUG_BUILD

#ifdef DEBUG_BUILD
#define DEBUG(x) do { std::cerr << x << endl; } while (0)
#else
#define DEBUG(x) do {} while (0)
#endif

// TODO: reference additional headers your program requires here
