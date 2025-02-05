// Copyright (C) 2024 kbz_8 ( contact@kbz8.me )
// This file is a part of Akel
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef __AK_RHI_DEVICE__
#define __AK_RHI_DEVICE__

#include <Graphics/PreCompiled.h>
#include <Utils/NonOwningPtr.h>
#include <Graphics/RHI/Enums.h>
#include <Graphics/RHI/Defs.h>

namespace Ak
{
	class RHIDevice
	{
		public:
			RHIDevice() = default;

			virtual NonOwningPtr<class RHIBuffer> CreateBuffer(BufferDescription description) = 0;
			virtual NonOwningPtr<class RHITexture> CreateTexture(TextureDescription description) = 0;
			virtual NonOwningPtr<class RHIRenderSurface> CreateRenderSurface() noexcept = 0;
			virtual NonOwningPtr<class RHIGraphicPipeline> CreateGraphicPipeline() noexcept = 0;

			virtual void WaitForIdle() = 0;

			virtual ~RHIDevice() = default;
	};
}

#endif
