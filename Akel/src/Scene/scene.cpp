// This file is a part of Akel
// Authors : @kbz_8
// Created : 05/12/2022
// Updated : 19/12/2022

#include <Scene/scene.h>
#include <Platform/window.h>
#include <Graphics/matrixes.h>
#include <Renderer/Buffers/vk_ubo.h>

namespace Ak
{
	struct MatrixesBuffer
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	Scene::Scene(fString name, WindowComponent* window) : _name(std::move(name)), _window(window)
	{
		if(window == nullptr)
			Core::log::report(FATAL_ERROR, "Scene '%s' : wrong window pointer", name.c_str());
	}

	void Scene::onAttach(uint32_t id) noexcept
	{
		_id = id;
		_2D_pipeline.init(_shaders, std::vector<Ak::Shader::VertexInput>{ {
				{ Vertex2D::getBindingDescription() },
				{ Vertex2D::getAttributeDescriptions()[0], Vertex2D::getAttributeDescriptions()[1] }
		} });

		_3D_pipeline.init(_shaders, std::vector<Ak::Shader::VertexInput>{ {
				{ Vertex3D::getBindingDescription() },
				{ Vertex3D::getAttributeDescriptions()[0], Vertex3D::getAttributeDescriptions()[1] }
		} }, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
	}

	void Scene::add_2D_entity(Entity2D entity)
	{
		entity.initBuffers();
		_2D_entities.push_back(std::move(entity));
	}

	void Scene::add_3D_entity(Entity3D entity)
	{
		entity.initBuffers();
		_3D_entities.push_back(std::move(entity));
	}

	void Scene::onPreRender()
	{
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
	}

	void Scene::onRender2D()
	{
		if(_2D_pipeline.getShaders().size() == 0 || _2D_entities.size() == 0)
			return;

		_2D_pipeline.bindPipeline(Render_Core::get().getActiveCmdBuffer());

		Matrixes::ortho(0, _window->size.X, 0, _window->size.Y);
	
		for(Shader* shader : _2D_pipeline.getShaders())
		{
			if(shader->getUniforms().size() > 0)
			{
				if(shader->getUniforms().count("matrixes"))
				{
					Matrixes::matrix_mode(matrix::view);
					Matrixes::load_identity();
					Matrixes::matrix_mode(matrix::model);
					Matrixes::load_identity();

					MatrixesBuffer mat;
					mat.proj = Matrixes::get_matrix(matrix::proj);
					mat.model = Matrixes::get_matrix(matrix::model);
					mat.view = Matrixes::get_matrix(matrix::view);

					shader->getUniforms()["matrixes"].getBuffer()->setData(sizeof(mat), &mat);
				}

				vkCmdBindDescriptorSets(Render_Core::get().getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _2D_pipeline.getPipelineLayout(), 0, 1, shader->getVkDescriptorSets().data(), 0, nullptr);
			}
		}

        for(Entity2D& ent : _2D_entities)
		{
			ent._vbo.bind();
			ent._ibo.bind();

			vkCmdDrawIndexed(Render_Core::get().getActiveCmdBuffer().get(), static_cast<uint32_t>(ent._ibo.getSize() / sizeof(uint32_t)), 1, 0, 0, 0);
		}
	}

	void Scene::onRender3D()
	{
		if(_3D_pipeline.getShaders().size() == 0 || _3D_entities.size() == 0)
			return;

		_3D_pipeline.bindPipeline(Render_Core::get().getActiveCmdBuffer());

		Matrixes::perspective(90.f, (float)_window->size.X / (float)_window->size.Y, 0.1f, 1000.f);
	
		for(Shader* shader : _3D_pipeline.getShaders())
		{
			if(shader->getUniforms().size() > 0)
			{
				if(shader->getUniforms().count("matrixes"))
				{
					Matrixes::matrix_mode(matrix::model);
					Matrixes::load_identity();

					MatrixesBuffer mat;
					mat.proj = Matrixes::get_matrix(matrix::proj);
					mat.model = Matrixes::get_matrix(matrix::model);
					mat.view = Matrixes::get_matrix(matrix::view);

					mat.proj[1][1] *= -1;

					shader->getUniforms()["matrixes"].getBuffer()->setData(sizeof(mat), &mat);
				}

				vkCmdBindDescriptorSets(Render_Core::get().getActiveCmdBuffer().get(), VK_PIPELINE_BIND_POINT_GRAPHICS, _3D_pipeline.getPipelineLayout(), 0, 1, shader->getVkDescriptorSets().data(), 0, nullptr);
			}
		}

        for(Entity3D& ent : _3D_entities)
		{
			ent._vbo.bind();
			ent._ibo.bind();

			vkCmdDrawIndexed(Render_Core::get().getActiveCmdBuffer().get(), static_cast<uint32_t>(ent._ibo.getSize() / sizeof(uint32_t)), 1, 0, 0, 0);
		}

	}

	void Scene::onQuit()
	{
		for(auto entity : _2D_entities)
			entity.destroy();
		for(auto entity : _3D_entities)
			entity.destroy();
		_2D_pipeline.destroy();
		_3D_pipeline.destroy();
	}
}
