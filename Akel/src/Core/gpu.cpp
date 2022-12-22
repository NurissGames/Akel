// This file is a part of Akel
// Authors : @kbz_8
// Created : 23/04/2021
// Updated : 22/12/2022

#include <Core/core.h>
#include <Utils/utils.h>

namespace Ak::Core
{
    GPU::GPU()
    {   
        _vendors[0x1002] = "AMD";
        _vendors[0x1010] = "ImgTec";
        _vendors[0x10DE] = "Nvidia";
        _vendors[0x13B5] = "ARM";
        _vendors[0x5143] = "Qualcomm";
        _vendors[0x8086] = "Intel";

        _types[0] = "Other";
        _types[1] = "Integrated";
        _types[2] = "Graphics Card";
        _types[3] = "Virtual";
        _types[4] = "CPU";

        VkInstanceCreateInfo createInfo{};
        createInfo.enabledLayerCount = 0;

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		volkInitializeCustom((PFN_vkGetInstanceProcAddr)vkGetInstanceProcAddr(nullptr, "vkCreateInstance"));
		volkInitializeCustom((PFN_vkGetInstanceProcAddr)vkGetInstanceProcAddr(nullptr, "vkEnumeratePhysicalDevices"));
		volkInitializeCustom((PFN_vkGetInstanceProcAddr)vkGetInstanceProcAddr(nullptr, "vkGetPhysicalDeviceProperties"));

        if(vkCreateInstance(&createInfo, nullptr, &_instance) != VK_SUCCESS)
            Core::log::report(ERROR, "Unable to get GPU info, unable to create vulkan instance");
        else
        {
            vkEnumeratePhysicalDevices(_instance, &_deviceCount, nullptr);
            _devices.resize(_deviceCount);
            vkEnumeratePhysicalDevices(_instance, &_deviceCount, _devices.data());
            vkGetPhysicalDeviceProperties(_devices[0], &_deviceProperties);
        }

        auto FN_vkEnumerateInstanceVersion = PFN_vkEnumerateInstanceVersion(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
        if(vkEnumerateInstanceVersion)
            vkEnumerateInstanceVersion(&_instanceVersion);
    }

    GPU::~GPU()
    {
        if(_instance)
            vkDestroyInstance(_instance, nullptr);
    }
}
