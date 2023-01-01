// This file is a part of Akel
// Authors : @kbz_8
// Created : 20/08/2022
// Updated : 27/12/2022

#ifndef __AK_ASSERT__
#define __AK_ASSERT__

#include <Core/profile.h>

#ifdef AK_RELEASE
    #define assert(con) (void)(0)
    #define Ak_assert(con, msg) (void)(0)
#else
    namespace Ak { void FatalError(std::string message, ...); }
    #define Ak_assert(con, msg) \
		if(bool(con)) {} \
		else \
		{ \
			Ak::FatalError("Assertion failed (in " __FILE__ ", in '%s', at line %d) with message : %s", AK_FUNC_SIG, __LINE__, msg); \
		}
#endif

#endif // __AK_ASSERT__
