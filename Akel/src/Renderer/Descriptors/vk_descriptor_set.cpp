// This file is a part of Akel
// Authors : @kbz_8
// Created : 12/04/2022
// Updated : 08/12/2022

#include "vk_descriptor_set.h"
#include "vk_descriptor_set_layout.h"
#include "vk_descriptor_pool.h"
#include <Renderer/Buffers/vk_ubo.h>
#include <Renderer/Core/render_core.h>
#include <Renderer/Pipeline/vk_shader.h>
#include <Utils/assert.h>

namespace Ak
{
    void DescriptorSet::init(UBO* ubo, DescriptorSetLayout& layout, DescriptorPool& pool)
    {
        auto device = Render_Core::get().getDevice().get();

		_pool = pool.get();

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = _pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout.get();

        if(vkAllocateDescriptorSets(device, &allocInfo, &_desc_set) != VK_SUCCESS)
            Core::log::report(FATAL_ERROR, "Vulkan : failed to allocate descriptor set");

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = ubo->get();
        bufferInfo.offset = ubo->getOffset();
        bufferInfo.range = ubo->getSize();

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = _desc_set;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
    }

    void DescriptorSet::destroy() noexcept
    {
        Ak_assert(_desc_set != VK_NULL_HANDLE, "trying to destroy an uninit descriptor set");
        vkFreeDescriptorSets(Render_Core::get().getDevice().get(), _pool, 1, &_desc_set);
    }
}
