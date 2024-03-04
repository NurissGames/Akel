// This file is a part of Akel
// Authors : @kbz_8
// Created : 31/01/2024
// Updated : 04/03/2024

#ifndef __AK_CORE_COREMODULE__
#define __AK_CORE_COREMODULE__

#include <Core/PreCompiled.h>
#include <Core/Module.h>
#include <Core/Ticks.h>
#include <Utils/TypeList.h>

namespace Ak
{
	class AK_CORE_API CoreModule : public Module
	{
		public:
			using Dependencies = TypeList<>;

			CoreModule();

			static inline bool IsInit() noexcept { return s_instance != nullptr; }
			static CoreModule& Get();

			inline Ticks& GetTicksReference() { return m_ticks; }

			~CoreModule() override;

		private:
			static CoreModule* s_instance;
			Ticks m_ticks;
	};
}

#endif
