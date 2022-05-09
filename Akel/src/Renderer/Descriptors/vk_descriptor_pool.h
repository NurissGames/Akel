// This file is a part of Akel
// Authors : @kbz_8
// Created : 12/04/2022
// Updated : 09/05/2022

#ifndef __AK_VK_DESCRIPTOR_POOL__
#define __AK_VK_DESCRIPTOR_POOL__

#include <Akpch.h>

namespace Ak
{
    class DescriptorPool
    {
        public:
            void init();
            void destroy() noexcept;

            inline VkDescriptorPool& operator()() noexcept { return _pool; }
            inline VkDescriptorPool& get() noexcept { return _pool; }

        private:
            VkDescriptorPool _pool = VK_NULL_HANDLE;
    };
}

#endif // __AK_VK_DESCRIPTOR_POOL__
