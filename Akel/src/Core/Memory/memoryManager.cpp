// This file is a part of Akel
// CREATED : 23/07/2021
// UPDATED : 22/02/2022

#include <Utils/utils.h>
#include <Core/core.h>

namespace Ak
{
    void MemoryManager::init()
    {
        control_unit = std::make_shared<ControlUnit>();
        if(_use)
        {
        #ifndef AK_USE_JAM_MEMORY_HELPER
            __fixed1.init(16, 100);
            __fixed2.init(32, 200);
            __fixed3.init(96, 400);

            __fixed1.autoResize(true);
            __fixed2.autoResize(true);
            __fixed3.autoResize(true);
        #endif
            __jam.init(4096 * 4096);
            __jam.auto_increase_size(true);
        }
    }
    void MemoryManager::end()
    {
        if(_use)
        {
        #ifndef AK_USE_JAM_MEMORY_HELPER
            __fixed1.destroy();
            __fixed2.destroy();
            __fixed3.destroy();
        #endif
            __jam.destroy();
        }
    }

    void MemoryManager::useMemoryManager(bool set)
    {
        _use = set;
		Core::ProjectFile::setBoolValue("use_memory_manager", _use);
    }
}
