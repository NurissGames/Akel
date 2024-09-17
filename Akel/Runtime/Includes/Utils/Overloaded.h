// Copyright (C) 2024 kbz_8 ( contact@kbz8.me )
// This file is a part of Akel
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef __AK_UTILS_OVERLOAD__
#define __AK_UTILS_OVERLOAD__

namespace Ak
{
	template<class... Ts>
	struct Overloaded : Ts...
	{
		using Ts::operator()...;
	};

	template<class... Ts>
	Overloaded(Ts...) -> Overloaded<Ts...>;
}

#endif
