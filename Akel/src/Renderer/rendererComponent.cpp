// This file is a part of Akel
// Authors : @kbz_8
// Created : 23/09/2021
// Updated : 06/12/2022

#include <Renderer/rendererComponent.h>
#include <Renderer/Buffers/vk_ubo.h>

namespace Ak
{
    RendererComponent::RendererComponent(WindowComponent* window) : Component("__renderer_component"), _window(window) {}

    void RendererComponent::onAttach()
    {
        Render_Core::get().setWindow(_window);
        Render_Core::get().init();

		if(!_shaders.empty())
		{
			_pipeline.init(_shaders, std::vector<Ak::Shader::VertexInput>{ {
					{ Vertex2D::getBindingDescription() },
					{ Vertex2D::getAttributeDescriptions()[0], Vertex2D::getAttributeDescriptions()[1] }
			} });
		}
    }

	void RendererComponent::update()
	{
		if(_reload)
		{
			if(!_shaders.empty())
			{
				_pipeline.destroy();
				_pipeline.init(_shaders, std::vector<Ak::Shader::VertexInput>{ {
						{ Vertex2D::getBindingDescription() },
						{ Vertex2D::getAttributeDescriptions()[0], Vertex2D::getAttributeDescriptions()[1] }
				} }, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, _cull_mode);
			}
			_reload = false;
		}
	}

    void RendererComponent::onRender()
    {
		if(_shaders.empty())
			return;

		_pipeline.bindPipeline(Render_Core::get().getActiveCmdBuffer());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)Render_Core::get().getSwapChain()._swapChainExtent.width;
		viewport.height = (float)Render_Core::get().getSwapChain()._swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(Render_Core::get().getActiveCmdBuffer().get(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = Render_Core::get().getSwapChain()._swapChainExtent;
		vkCmdSetScissor(Render_Core::get().getActiveCmdBuffer().get(), 0, 1, &scissor);

		/*
		for(Shader* shader : _pipeline.getShaders())
		{
			for(UBO& ubo : shader->getUniformBuffers())
			{
				ubo.setDynamicData();
			}
		}
		*/
    }

    void RendererComponent::onQuit()
    {
        Render_Core::get().getSwapChain().destroyFB();
		Render_Core::get().destroyCommandBuffers();
		if(!_shaders.empty())
			_pipeline.destroy();
        Render_Core::get().getSwapChain().destroy();
    }
}
