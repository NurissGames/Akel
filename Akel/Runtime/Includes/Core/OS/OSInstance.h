// This file is a part of Akel
// Authors : @kbz_8
// Created : 02/02/2024
// Updated : 04/02/2024

#ifndef __AK_CORE_OS_INSTANCE__
#define __AK_CORE_OS_INSTANCE__

#include <Core/PreCompiled.h>
#include <Maths/Vec4.h>

namespace Ak
{
	class OSInstance
	{
		public:
			virtual void Init() {}
			virtual void Shutdown() = 0;

			virtual std::filesystem::path GetExecutablePath() = 0;
			virtual std::filesystem::path GetCurrentWorkingDirectoryPath() = 0;
			virtual bool OpenURL([[maybe_unused]] const std::string& url) { return false; }
			virtual void Delay(std::uint32_t us) {}

			virtual bool SetTitleBarColor([[maybe_unused]] Vec4f color, [[maybe_unused]] bool dark = true) {}

			virtual OSInstance& Get() = 0;

		protected:
			OSInstance() = default;
			virtual ~OSInstance() = default;
	};
}

#endif
