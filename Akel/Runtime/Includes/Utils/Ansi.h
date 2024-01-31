// This file is a part of Akel
// Authors : @kbz_8
// Created : 31/01/2024
// Updated : 31/01/2024

#ifndef __AK_UTILS_ANSI__
#define __AK_UTILS_ANSI__

#include <Utils/PreCompiled.h>

namespace Ak
{
	enum class Ansi : std::uint32_t
	{
		red = 31,
		green = 32,
		blue = 34,
		def = 0,
		black = 30,
		yellow = 33,
		magenta = 35,
		cyan = 36,
		white = 37,
		bg_red = 41,
		bg_green = 42,
		bg_blue = 44,
		bg_def = 0,
		bg_black = 40,
		bg_yellow = 43,
		bg_magenta = 45,
		bg_cyan = 46,
		bg_white = 47,
		reset = 0,
		bold = 1,
		underline = 4,
		inverse = 7,
		bold_off = 21,
		underline_off = 24,
		inverse_off = 27
	};

	std::ostream &operator<<(std::ostream& o1s, Ansi ansi);
}

#endif
