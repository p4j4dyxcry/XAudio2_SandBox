#pragma once

#include <string>
#include <vector>
#include <list>
#include <stack>
#include <unordered_map>

typedef unsigned int        TsUint;
typedef int                 TsInt;
typedef float               TsF32;
typedef double              TsF64;
typedef wchar_t             TsLChar;
typedef char                TsChar;
typedef char                TsS8;
typedef unsigned char       TsU8;
typedef short               TsShort;
typedef unsigned short      TsUshort;
typedef TsU8                TsByte;
typedef bool                TsBool;
typedef long long           TsS64;
typedef unsigned long long  TsU64;
typedef std::string         TsString;

#if USE_EASTL
#define tstl               eastl
#else
#define tstl               std
#endif

#define TsList              tstl::list
#define TsVector            tstl::vector
#define TsStack             tstl::stack


#define TsMap               std::unordered_map

#define TS_TRUE             true
#define TS_FALSE            false

typedef TsU64 TS_HASH;