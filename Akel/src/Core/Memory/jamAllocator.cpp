// This file is a part of Akel
// CREATED : 20/07/2021
// UPDATED : 26/12/2021

#include <Core/core.h>

namespace Ak
{
    JamAllocator::JamAllocator() : _usedSpaces(), _freeSpaces() {}

    void JamAllocator::init(size_t Size)
    {
        if(_heap != nullptr)
            return;

        lockThreads(mutex);

        #if defined(AK_PLATFORM_WINDOWS) && defined(_MSC_VER) && _MSC_VER < 1900
              InitializeCriticalSection(&mutex);
        #endif

        _heap = std::malloc(Size);

        if(!_heap)
            Core::log::report(FATAL_ERROR, "JamAllocator : unable to allocate memory space for an allocator");

        _heapSize = Size;
        _memUsed = 0;
        _heapEnd = (void*)(reinterpret_cast<uintptr_t>(_heap) + _heapSize);

        _allocator_number = MemoryManager::accessToControlUnit()->jamStack.size();
        std::string key = "jamAllocator_size_" + std::to_string(_allocator_number);
        Core::ProjectFile::setIntValue(key, Size);
        MemoryManager::accessToControlUnit()->jamStack.push_back(weak_from_this());

        unlockThreads(mutex);
    }

    void JamAllocator::increase_size(size_t Size)
    {
        if(Size < _heapSize)
        {
            Core::log::report(WARNING, "JamAllocator : allocators are not supposed to reduce their size");
            return;
        }

        lockThreads(mutex);

        if(Size > _heapSize)
        {
            _resises.push_back(std::make_pair(std::malloc(Size - _heapSize), Size - _heapSize));

            _heapSize = Size;
            std::string key = "jamAllocator_size_" + std::to_string(_allocator_number);
            Core::ProjectFile::setIntValue(key, Size);
        }

        unlockThreads(mutex);
    }

    inline bool JamAllocator::canHold(size_t Size)
    {
        return Size > _heapSize - _memUsed;
    }

    inline void JamAllocator::auto_increase_size(bool set)
    {
        _autoResize = set;
    }

    inline bool JamAllocator::contains(void* ptr)
    {
    	return ptr > _heap && ptr < _heapEnd;
    }

    void JamAllocator::destroy()
    {
        if(_heap == nullptr)
            return;

        lockThreads(mutex);

        std::free(_heap);
        _heap = nullptr;
        
        std::string key = "jamAllocator_size_" + std::to_string(_allocator_number);
        Core::ProjectFile::setIntValue(key, _memUsed);

        unlockThreads(mutex);
    }

    JamAllocator::~JamAllocator()
    {
        destroy();

        #if defined(AK_PLATFORM_WINDOWS) && defined(_MSC_VER) && _MSC_VER < 1900
            DeleteCriticalSection(&mutex);
        #endif
    }
}
