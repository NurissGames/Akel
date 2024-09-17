// Copyright (C) 2024 kbz_8 ( contact@kbz8.me )
// This file is a part of Akel
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef __AK_DRIVERS_UNIX_PRE_COMPILED_HEADER__
#define __AK_DRIVERS_UNIX_PRE_COMPILED_HEADER__

#include <Core/CompilationProfile.h>
#include <Drivers/Unix/Unix.h>
#include <ctime>
#ifdef AK_PLAT_UNIX
	#include <unistd.h>
	#include <signal.h>
	#include <dlfcn.h>
#endif

#endif
