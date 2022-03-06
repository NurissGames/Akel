// This file is a part of Akel
// Authors : @kbz_8
// Created : 24/09/2021
// Updated : 10/12/2021

#ifndef __AK_VULKAN_GPU_MEMORY_CHUNK__
#define __AK_VULKAN_GPU_MEMORY_CHUNK__

#include <Akpch.h>

namespace Ak
{
    struct Buffer
    {
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkDeviceSize offset = 0;
        VkDeviceSize size = 0;
        VkBuffer buffer = VK_NULL_HANDLE;
        void* ptr = nullptr;

        bool operator == (Buffer const& buffer);
    };

    class Chunk
    {
        public:
            explicit Chunk(VkDeviceSize size, int memoryTypeIndex);

            bool alloc(VkDeviceSize size, Buffer& buffer, VkDeviceSize alignment = 1);
            inline bool contains(const Buffer& buffer);
            inline bool canHold(VkDeviceSize size);
            void free(const Buffer& buffer);
            inline int memoryTypeIndex() const noexcept { return _memoryTypeIndex; }

        private:
            VkDeviceMemory _memory = VK_NULL_HANDLE;
            VkDeviceSize _size;
            VkDeviceSize _memUsed = 0;
            int _memoryTypeIndex = 0;
            std::vector<Buffer> _usedBuffers;
            std::vector<Buffer> _freeBuffers;
            void *_ptr = nullptr;
    };
}

#endif // __AK_VULKAN_GPU_MEMORY_CHUNK__
