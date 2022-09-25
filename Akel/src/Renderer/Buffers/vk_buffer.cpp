// This file is a part of Akel
// Authors : @kbz_8
// Created : 10/04/2022
// Updated : 24/09/2022

#include "vk_buffer.h"
#include <Utils/assert.h>

namespace Ak
{
	void Buffer::create(Buffer::kind type, VkDeviceSize size, VkBufferUsageFlags usage, const void* data)
	{
		if(type == Buffer::kind::constant)
		{
			if(data == nullptr)
			{
				Core::log::report(ERROR, "Vulkan : trying to create constant buffer without data (constant buffers cannot be modified after creation)");
				return;
			}
			_usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else
		{
			_usage = usage;
			_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		_mem_chunck.size = size;

		createBuffer(_usage, _flags);

		if(type == Buffer::kind::constant)
		{
			void* mapped = nullptr;
			mapMem(mapped);
				std::memcpy(mapped, data, _mem_chunck.size);
			unmapMem();

			pushToGPU();
		}
	}

	void Buffer::destroy() noexcept
	{
		Ak_assert(_buffer != VK_NULL_HANDLE, "trying to destroy an uninit video buffer");
		vkDestroyBuffer(Render_Core::get().getDevice().get(), _buffer, nullptr);
		Render_Core::get().freeChunk(_mem_chunck);
	}

	void Buffer::createBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _mem_chunck.size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		auto device = Render_Core::get().getDevice().get();

		if(vkCreateBuffer(device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
			Core::log::report(FATAL_ERROR, "Vulkan : failed to create buffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, _buffer, &memRequirements);

		_mem_chunck = Render_Core::get().allocChunk(memRequirements, properties);

		if(vkBindBufferMemory(device, _buffer, _mem_chunck.memory, _mem_chunck.offset) != VK_SUCCESS)
			Core::log::report(FATAL_ERROR, "Vulkan : unable to bind device memory to a buffer object");

		Ak_assert(_buffer != VK_NULL_HANDLE, "Vulkan : something went wrong in the creation of a buffer");
	}

	void Buffer::pushToGPU() noexcept
	{
		Buffer newBuffer;
		newBuffer._mem_chunck.size = this->_mem_chunck.size;
		newBuffer._usage = (this->_usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		newBuffer._flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		newBuffer.createBuffer(newBuffer._usage, newBuffer._flags);

		auto cmdpool = Render_Core::get().getCmdPool().get();
		auto device = Render_Core::get().getDevice().get();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = cmdpool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.size = this->_mem_chunck.size;
		vkCmdCopyBuffer(commandBuffer, _buffer, newBuffer._buffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		auto graphicsQueue = Render_Core::get().getQueue().getGraphic();

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(device, cmdpool, 1, &commandBuffer);

		this->swap(newBuffer);

		newBuffer.destroy();
	}

	void Buffer::swap(Buffer& buffer)
	{
		VkBuffer temp_b = _buffer;
		_buffer = buffer._buffer;
		buffer._buffer = temp_b;

		GPU_Mem_Chunk temp_c = _mem_chunck;
		_mem_chunck = buffer._mem_chunck;
		buffer._mem_chunck = temp_c;

		VkBufferUsageFlags temp_u = _usage;
		_usage = buffer._usage;
		buffer._usage = temp_u;

		VkMemoryPropertyFlags temp_f = _flags;
		_flags = buffer._flags;
		buffer._flags = temp_f;
	}

	void Buffer::flush(VkDeviceSize size, VkDeviceSize offset)
	{
		VkMappedMemoryRange mappedRange{};
		mappedRange.memory = _mem_chunck.memory;
		mappedRange.offset = offset;
		mappedRange.size = size;
		vkFlushMappedMemoryRanges(Render_Core::get().getDevice().get(), 1, &mappedRange);
	}

	uint32_t Buffer::findMemoryType(uint32_t typeFilter)
    {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(Render_Core::get().getDevice().getPhysicalDevice(), &memProperties);

		for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		{
			if((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & _flags) == _flags)
				return i;
		}

		Core::log::report(FATAL_ERROR, "Vulkan : failed to find suitable memory type");
		return 0; // Not necessary due to the FATAL_ERROR parameter for logs but that removes a warning
	}
}
