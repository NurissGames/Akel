// This file is a part of Akel
// Authors : @kbz_8
// Created : 04/04/2022
// Updated : 19/12/2022

#ifndef __AK_VK_GRAPHIC_PIPELINE__
#define __AK_VK_GRAPHIC_PIPELINE__

#include "vk_pipeline.h"
#include "vk_shader.h"

namespace Ak
{
	class GraphicPipeline : public Pipeline
	{
		public:
			void init(	std::vector<std::vector<uint32_t>> shaders, std::vector<Shader::VertexInput> inputs,
						VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL, 
						VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT, VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE);
			void destroy() noexcept;

			inline const std::vector<Shader*>& getShaders() const override { return _shaders; }
			inline const VkPipeline& getPipeline() const override { return _graphicsPipeline; }
			inline const VkPipelineLayout& getPipelineLayout() const override { return _pipelineLayout; }
			inline const VkPipelineBindPoint& getPipelineBindPoint() const override { return _pipelineBindPoint; }

		private:
			VkPipeline _graphicsPipeline = VK_NULL_HANDLE;
			VkPipelineCache _cache = VK_NULL_HANDLE;
			VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
			VkPipelineBindPoint _pipelineBindPoint;
			std::vector<Shader*> _shaders;
	};
}

#endif // __AK_VK_GRAPHIC_PIPELINE__
