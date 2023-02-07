// This file is a part of Akel
// Authors : @kbz_8
// Created : 12/04/2022
// Updated : 07/02/2023

#ifndef __AK_VK_DESCRIPTOR_SET__
#define __AK_VK_DESCRIPTOR_SET__

#include <Akpch.h>

namespace Ak
{
    class DescriptorSet
    {
        public:
            void init(class RendererComponent* renderer, class DescriptorSetLayout& layout, class DescriptorPool& pool);

			void writeDescriptor(int binding, class UBO* ubo) noexcept;
			void writeDescriptor(int binding, VkImageView view, VkSampler sampler) noexcept;

            void destroy() noexcept;

            VkDescriptorSet& operator()() noexcept;
            VkDescriptorSet& get() noexcept;

        private:
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> _desc_set;
			VkDescriptorPool _pool = VK_NULL_HANDLE;
			class RendererComponent* _renderer = nullptr;
    };
}

#endif // __AK_VK_DESCRIPTOR_SET__
