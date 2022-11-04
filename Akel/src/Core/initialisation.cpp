// This file is a part of Akel
// Authors : @kbz_8
// Created : 06/10/2021
// Updated : 04/11/2022

#include <Audio/audio.h>
#include <Renderer/Core/render_core.h>
#include "softwareInfo.h"

namespace Ak
{
	namespace Core
	{
		std::string getMainDirPath();
		void initCpuInfo();
	}
	
    bool initAkel(AkelInstance* project)
    {
		#if defined(AK_64BITS)
			if(sizeof(void*) != 8)
			{
				Core::log::report(ERROR, "Conflict of system architecture detection");
			    return false;
			}
			Core::log::report("architecture: 64bits");
		#elif defined(AK_32BITS)
			if(sizeof(void*) != 4)
			{
				Core::log::report(ERROR, "Conflict of system architecture detection");
			    return false;
			}
			Core::log::report("architecture: 32bits");
		#endif

		if(std::strcmp(AK_arch, "x86") != 0)
		{
			Core::log::report(ERROR, "Akel can only run on x86 architecture, you are trying to run it on a " AK_arch " architecture");
			return false;
		}
		Core::log::report("arch: " AK_arch);

		if(project->app == nullptr)
		{
			Core::log::report(ERROR, "Application pointer is null");
			return false;
		}

		if(!Core::isVulkanSupported())
		{
			Core::log::report(ERROR, "Vulkan is not supported");
			return false;
		}

		if(project->project_file_name != "")
			Core::ProjectFile::setName(project->project_file_name);
		if(project->project_file_path != "")
			Core::ProjectFile::setDir(project->project_file_path);
		else
			Core::ProjectFile::setDir(Core::getMainDirPath());
		
		Core::ProjectFile::initProjFile();

		Core::ProjectFile::setBoolValue("enable_warning_console_message", project->enable_warning_console_message);
		Core::ProjectFile::setBoolValue("vk_enable_message_validation_layer", project->vk_enable_message_validation_layer);
		Core::ProjectFile::setBoolValue("vk_force_disable_validation_layers", project->vk_force_disable_validation_layers);
		Core::ProjectFile::setBoolValue("memory_manager_enable_fixed_allocator", project->memory_manager_enable_fixed_allocator);

		MemoryManager::useMemoryManager(project->use_memory_manager);
		MemoryManager::init();

		Core::initCpuInfo();

		return true;
    }
}
