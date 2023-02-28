// This file is a part of Akel
// Authors : @kbz_8
// Created : 15/02/2023
// Updated : 28/02/2023

#include <Renderer/scene_renderer.h>
#include <Renderer/rendererComponent.h>
#include <Scene/Attributes/attributes.h>
#include <Scene/Cameras/base_camera.h>
#include <Scene/entity.h>
#include <Renderer/Buffers/vk_ubo.h>

namespace Ak
{
	struct MatricesBuffer
	{
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	void SceneRenderer::init(SceneRendererSettings settings)
	{
		_settings = std::move(settings);
	}

	void SceneRenderer::render(Scene* scene)
	{
		auto renderer = scene->_renderer;

		if(_settings.geometries)
		{
			if(_scene_cache != scene)
			{
				_forward_data.shaders.clear();
				_forward_data.shaders = scene->_forward_shaders;
			}
			
			_forward_data.command_queue.clear();
			auto world = scene->getRegistry().view<ModelAttribute>();
			for(auto e : world)
			{
				ModelAttribute model = world.get<ModelAttribute>(e);

				RenderCommandData command;
				command.mesh = model.model.getMesh().get();
				command.material = model.model.getMaterial();

				_forward_data.command_queue.push_back(command);
			}
			forwardPass(scene);
		}
		if(_scene_cache != scene)
			_scene_cache = scene;
	}

	void SceneRenderer::forwardPass(Scene* scene)
	{
		auto renderer = scene->_renderer;

		PipelineDesc pipeline_desc;
		pipeline_desc.shaders = _forward_data.shaders;

		auto pipeline = _pipelines_manager.getPipeline(*renderer, pipeline_desc);
		if(pipeline == nullptr)
			return;

		// caches
		static std::vector<VkDescriptorSet> sets;
		static Shader::Uniform matrices_uniform_buffer;
		static ShaderID fragment_shader = nullshader;

		if(scene != _scene_cache)
		{
			sets.clear();
			for(ShaderID id : _forward_data.shaders)
			{
				auto shader = ShadersLibrary::get().getShader(id);
				if(shader->getType() == VK_SHADER_STAGE_FRAGMENT_BIT)
					fragment_shader = id;
				for(DescriptorSet& set : shader->getDescriptorSets())
					sets.push_back(set.get());
				if(shader->getUniforms().size() > 0)
				{
					if(shader->getUniforms().count("matrices"))
						matrices_uniform_buffer = shader->getUniforms()["matrices"];
				}
			}
		}

		MatricesBuffer mat;
		mat.proj = scene->_camera->getProj();
		mat.view = scene->_camera->getView();
		mat.proj[1][1] *= -1;
		matrices_uniform_buffer.getBuffer()->setData(sizeof(mat), &mat);

		pipeline->bindPipeline(renderer->getActiveCmdBuffer());
		renderer->getRenderPass().begin();

		for(RenderCommandData& command : _forward_data.command_queue)
		{
			if(fragment_shader != nullshader)
			{
				auto material = MaterialLibrary::get().getMaterial(command.material);
				material->updateDescriptors(fragment_shader);
			}

			vkCmdBindDescriptorSets(renderer->getActiveCmdBuffer().get(), pipeline->getPipelineBindPoint(), pipeline->getPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
			command.mesh->draw(*renderer);
		}

		renderer->getRenderPass().end();
	}

	void SceneRenderer::destroy()
	{
		_pipelines_manager.clearCache();
		TextureLibrary::get().clearLibrary();
	}
}
