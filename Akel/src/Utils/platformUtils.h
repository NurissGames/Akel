// This file is a part of Akel
// Authors : @kbz_8
// Created : 22/07/2021
// Updated : 17/09/2021

#ifndef __AK_PLATFORM_UTILS__
#define __AK_PLATFORM_UTILS__

#include <Akpch.h>
#include <Core/profile.h>

namespace Ak
{
    /**
     * Critical sections perform better in pre-2015 Visual Studio,
     * so it's best to use them. However, since Visual Studio 2015,
     * std::mutex are much more efficient than critical sections.
     */
    #if defined(AK_PLATFORM_WINDOWS) && defined(_MSC_VER) && _MSC_VER < 1900
        using MutexHandel = CRITICAL_SECTION;
    #else
        using MutexHandel = std::mutex;
    #endif

    void lockThreads(MutexHandel &lock);
    void unlockThreads(MutexHandel &lock);
}

#endif // __AK_PLATFORM_UTILS__
