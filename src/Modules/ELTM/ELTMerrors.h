// This file is a part of AtlasEngine
// CREATED : 13/05/2021
// UPDATED : 13/05/2021

#ifndef __ELTM_ERRORS__
#define __ELTM_ERRORS__

#include <AEpch.h>

namespace AE
{
	class ELTMerrors
	{
		public:
			ELTMerrors(std::string message, size_t line);
			const char* what();
			size_t line();

		private:
			size_t _line;
			std::string _message;
	};
}

#endif // __ELTM_ERRORS__

