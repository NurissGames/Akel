// This file is a part of Akel
// Authors : @kbz_8
// Created : 04/04/2022
// Updated : 03/11/2022

#include "vk_shader.h"
#include "vk_graphic_pipeline.h"
#include <Renderer/Core/render_core.h>
#include <Utils/assert.h>

#include <Utils/fStrings.h>

namespace Ak
{
	static uint32_t formatSize(VkFormat format)
	{
		switch(format)
		{
			case VK_FORMAT_UNDEFINED: return 0;
			case VK_FORMAT_R4G4_UNORM_PACK8: return 1;
			case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return 2;
			case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return 2;
			case VK_FORMAT_R5G6B5_UNORM_PACK16: return 2;
			case VK_FORMAT_B5G6R5_UNORM_PACK16: return 2;
			case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return 2;
			case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return 2;
			case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return 2;
			case VK_FORMAT_R8_UNORM: return 1;
			case VK_FORMAT_R8_SNORM: return 1;
			case VK_FORMAT_R8_USCALED: return 1;
			case VK_FORMAT_R8_SSCALED: return 1;
			case VK_FORMAT_R8_UINT: return 1;
			case VK_FORMAT_R8_SINT: return 1;
			case VK_FORMAT_R8_SRGB: return 1;
			case VK_FORMAT_R8G8_UNORM: return 2;
			case VK_FORMAT_R8G8_SNORM: return 2;
			case VK_FORMAT_R8G8_USCALED: return 2;
			case VK_FORMAT_R8G8_SSCALED: return 2;
			case VK_FORMAT_R8G8_UINT: return 2;
			case VK_FORMAT_R8G8_SINT: return 2;
			case VK_FORMAT_R8G8_SRGB: return 2;
			case VK_FORMAT_R8G8B8_UNORM: return 3;
			case VK_FORMAT_R8G8B8_SNORM: return 3;
			case VK_FORMAT_R8G8B8_USCALED: return 3;
			case VK_FORMAT_R8G8B8_SSCALED: return 3;
			case VK_FORMAT_R8G8B8_UINT: return 3;
			case VK_FORMAT_R8G8B8_SINT: return 3;
			case VK_FORMAT_R8G8B8_SRGB: return 3;
			case VK_FORMAT_B8G8R8_UNORM: return 3;
			case VK_FORMAT_B8G8R8_SNORM: return 3;
			case VK_FORMAT_B8G8R8_USCALED: return 3;
			case VK_FORMAT_B8G8R8_SSCALED: return 3;
			case VK_FORMAT_B8G8R8_UINT: return 3;
			case VK_FORMAT_B8G8R8_SINT: return 3;
			case VK_FORMAT_B8G8R8_SRGB: return 3;
			case VK_FORMAT_R8G8B8A8_UNORM: return 4;
			case VK_FORMAT_R8G8B8A8_SNORM: return 4;
			case VK_FORMAT_R8G8B8A8_USCALED: return 4;
			case VK_FORMAT_R8G8B8A8_SSCALED: return 4;
			case VK_FORMAT_R8G8B8A8_UINT: return 4;
			case VK_FORMAT_R8G8B8A8_SINT: return 4;
			case VK_FORMAT_R8G8B8A8_SRGB: return 4;
			case VK_FORMAT_B8G8R8A8_UNORM: return 4;
			case VK_FORMAT_B8G8R8A8_SNORM: return 4;
			case VK_FORMAT_B8G8R8A8_USCALED: return 4;
			case VK_FORMAT_B8G8R8A8_SSCALED: return 4;
			case VK_FORMAT_B8G8R8A8_UINT: return 4;
			case VK_FORMAT_B8G8R8A8_SINT: return 4;
			case VK_FORMAT_B8G8R8A8_SRGB: return 4;
			case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_UINT_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SINT_PACK32: return 4;
			case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_UINT_PACK32: return 4;
			case VK_FORMAT_A2R10G10B10_SINT_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_UINT_PACK32: return 4;
			case VK_FORMAT_A2B10G10R10_SINT_PACK32: return 4;
			case VK_FORMAT_R16_UNORM: return 2;
			case VK_FORMAT_R16_SNORM: return 2;
			case VK_FORMAT_R16_USCALED: return 2;
			case VK_FORMAT_R16_SSCALED: return 2;
			case VK_FORMAT_R16_UINT: return 2;
			case VK_FORMAT_R16_SINT: return 2;
			case VK_FORMAT_R16_SFLOAT: return 2;
			case VK_FORMAT_R16G16_UNORM: return 4;
			case VK_FORMAT_R16G16_SNORM: return 4;
			case VK_FORMAT_R16G16_USCALED: return 4;
			case VK_FORMAT_R16G16_SSCALED: return 4;
			case VK_FORMAT_R16G16_UINT: return 4;
			case VK_FORMAT_R16G16_SINT: return 4;
			case VK_FORMAT_R16G16_SFLOAT: return 4;
			case VK_FORMAT_R16G16B16_UNORM: return 6;
			case VK_FORMAT_R16G16B16_SNORM: return 6;
			case VK_FORMAT_R16G16B16_USCALED: return 6;
			case VK_FORMAT_R16G16B16_SSCALED: return 6;
			case VK_FORMAT_R16G16B16_UINT: return 6;
			case VK_FORMAT_R16G16B16_SINT: return 6;
			case VK_FORMAT_R16G16B16_SFLOAT: return 6;
			case VK_FORMAT_R16G16B16A16_UNORM: return 8;
			case VK_FORMAT_R16G16B16A16_SNORM: return 8;
			case VK_FORMAT_R16G16B16A16_USCALED: return 8;
			case VK_FORMAT_R16G16B16A16_SSCALED: return 8;
			case VK_FORMAT_R16G16B16A16_UINT: return 8;
			case VK_FORMAT_R16G16B16A16_SINT: return 8;
			case VK_FORMAT_R16G16B16A16_SFLOAT: return 8;
			case VK_FORMAT_R32_UINT: return 4;
			case VK_FORMAT_R32_SINT: return 4;
			case VK_FORMAT_R32_SFLOAT: return 4;
			case VK_FORMAT_R32G32_UINT: return 8;
			case VK_FORMAT_R32G32_SINT: return 8;
			case VK_FORMAT_R32G32_SFLOAT: return 8;
			case VK_FORMAT_R32G32B32_UINT: return 12;
			case VK_FORMAT_R32G32B32_SINT: return 12;
			case VK_FORMAT_R32G32B32_SFLOAT: return 12;
			case VK_FORMAT_R32G32B32A32_UINT: return 16;
			case VK_FORMAT_R32G32B32A32_SINT: return 16;
			case VK_FORMAT_R32G32B32A32_SFLOAT: return 16;
			case VK_FORMAT_R64_UINT: return 8;
			case VK_FORMAT_R64_SINT: return 8;
			case VK_FORMAT_R64_SFLOAT: return 8;
			case VK_FORMAT_R64G64_UINT: return 16;
			case VK_FORMAT_R64G64_SINT: return 16;
			case VK_FORMAT_R64G64_SFLOAT: return 16;
			case VK_FORMAT_R64G64B64_UINT: return 24;
			case VK_FORMAT_R64G64B64_SINT: return 24;
			case VK_FORMAT_R64G64B64_SFLOAT: return 24;
			case VK_FORMAT_R64G64B64A64_UINT: return 32;
			case VK_FORMAT_R64G64B64A64_SINT: return 32;
			case VK_FORMAT_R64G64B64A64_SFLOAT: return 32;
			case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return 4;
			case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return 4;

			default: return 0;
		}
	}

	std::vector<uint32_t> load_spirv_from_file(fString path)
	{
		std::ifstream stream(path.c_str(), std::ios::binary);

		if(!stream.is_open())
			Core::log::report(FATAL_ERROR, "Renderer : unable to open a spirv shader file");
		std::vector<uint32_t> data;

		stream.seekg(0);

		std::for_each(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>(), [&data](const char c){ data.push_back(static_cast<uint32_t>(c)); });
		
		stream.close();

		return data;
	}

	Shader::Shader(const std::vector<uint32_t> byte_code) : _byte_code(std::move(byte_code)) {}

	void Shader::generate()
	{
		SpvReflectShaderModule module = {};
		SpvReflectResult result = spvReflectCreateShaderModule(_byte_code.size() * sizeof(uint32_t), _byte_code.data(), &module);
		Ak_assert(result == SPV_REFLECT_RESULT_SUCCESS, "Renderer Shader : unable to create a Spir-V reflect shader module");

		uint32_t count = 0;
		result = spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
		
		std::vector<SpvReflectDescriptorSet*> sets(count);
		result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
		Ak_assert(result == SPV_REFLECT_RESULT_SUCCESS, "Renderer Shader : unable to enumerate descriptor sets");

		_entry_point_name = module.entry_point_name;

		_type = static_cast<VkShaderStageFlagBits>(module.shader_stage);

		for(int i = 0; i < sets.size(); i++)
		{
			auto set2 = spvReflectGetDescriptorSet(&module, sets[i]->set, &result);
			Ak_assert(result == SPV_REFLECT_RESULT_SUCCESS, "Renderer Shader : unable to get descriptor set");
			Ak_assert(sets[i] == set2, "Renderer Shader : somehting messed up while getting descripor set from a shader");

			for(int j = 0; j < sets[i]->binding_count; j++)
			{
				if(_uniforms.has(sets[i]->bindings[j]->name))
					return;
				if(sets[i]->bindings[j]->descriptor_type == SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
				{
					_uniforms.set_duet(sets[i]->bindings[j]->name,
						Shader::Uniform{
							static_cast<int32_t>(sets[i]->bindings[j]->binding),
							static_cast<int32_t>(sets[i]->bindings[j]->set),
							static_cast<int32_t>(sets[i]->bindings[j]->block.offset),
							static_cast<int32_t>(sets[i]->bindings[j]->block.size),
							_type
						});

					DescriptorSetLayout layout;
					layout.init(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, sets[i]->bindings[j]->binding, _type);
					_layouts.push_back(std::move(layout));
				}
			}
		}

		count = 0;
		result = spvReflectEnumerateInputVariables(&module, &count, NULL);
		Ak_assert(result == SPV_REFLECT_RESULT_SUCCESS, "Renderer Shader : unable to enumerate input variables");

		std::vector<SpvReflectInterfaceVariable*> input_vars(count);
		result = spvReflectEnumerateInputVariables(&module, &count, input_vars.data());
		Ak_assert(result == SPV_REFLECT_RESULT_SUCCESS, "Renderer Shader : unable to get input variables informations");

		uint32_t currentOffset = 4;

		for(std::size_t i = 0; i < input_vars.size(); i++)
		{
			VkVertexInputAttributeDescription attr_desc;
			attr_desc.binding = 0;
			attr_desc.location = input_vars[i]->location;
			attr_desc.format = static_cast<VkFormat>(input_vars[i]->format);
			attr_desc.offset = currentOffset;
			currentOffset += formatSize(attr_desc.format);
			
			_attributes.set_duet(input_vars[i]->name, std::move(attr_desc));
		}

		spvReflectDestroyShaderModule(&module);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = _byte_code.size() * sizeof(uint32_t);
		createInfo.pCode = _byte_code.data();

		if(vkCreateShaderModule(Render_Core::get().getDevice().get(), &createInfo, nullptr, &_shader) != VK_SUCCESS)
			Core::log::report(FATAL_ERROR, "Vulkan : failed to create shader module");
	}

	void Shader::destroy() noexcept
	{
		Ak_assert(_shader != VK_NULL_HANDLE, "trying to destroy an uninit shader");
		vkDestroyShaderModule(Render_Core::get().getDevice().get(), _shader, nullptr);
	}
}
