// This file is a part of Akel
// Authors : @kbz_8
// Created : 14/02/2023
// Updated : 27/02/2023

#ifndef __AK_RENDER_COMMAND_DATA__
#define __AK_RENDER_COMMAND_DATA__

#include <Akpch.h>
#include <Graphics/mesh.h>
#include <Graphics/material.h>
#include <Renderer/Pipeline/vk_graphic_pipeline.h>

namespace Ak
{
	struct RenderCommandData
	{
		glm::mat4 transform;
		Mesh* mesh = nullptr;
		MaterialID material = nullmaterial;
	};

	using CommandDataQueue = std::vector<RenderCommandData>;
}

#endif
