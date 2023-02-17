// This file is a part of Akel
// Authors : @kbz_8
// Created : 29/04/2022
// Updated : 17/02/2023

#ifndef __AK_VK_iBO__
#define __AK_VK_iBO__

#include <Renderer/rendererComponent.h>
#include "vk_buffer.h"

namespace Ak
{
	class IBO : public Buffer
	{
		public:
			inline void create(uint32_t size, const void* data = nullptr) { Buffer::create(Buffer::kind::dynamic, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data); }
			void setData(uint32_t size, const uint32_t* data);
			inline void bind(RendererComponent& renderer) noexcept { vkCmdBindIndexBuffer(renderer.getActiveCmdBuffer().get(), _buffer, _mem_chunck.offset, VK_INDEX_TYPE_UINT32); }
	};

	class C_IBO : public Buffer
	{
		public:
			inline void create(uint32_t size, const uint32_t* data) { Buffer::create(Buffer::kind::constant, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, data); }
			inline void bind(RendererComponent& renderer) noexcept { vkCmdBindIndexBuffer(renderer.getActiveCmdBuffer().get(), _buffer, _mem_chunck.offset, VK_INDEX_TYPE_UINT32); }
	};
}

#endif // __AK_VK_IBO__
