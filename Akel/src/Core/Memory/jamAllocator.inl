// This file is a part of Akel
// CREATED : 25/07/2021
// UPDATED : 15/01/2022

#include <Core/log.h>

namespace Ak
{
    template <typename T = void, typename ... Args>
    T* JamAllocator::alloc(Args&& ... args)
    {
        if(_heap == nullptr)
        {
            Core::log::report(FATAL_ERROR, "Jam Allocator: you need to initialize the allocator before aking him to give you memory");
            return nullptr;
        }
        size_t sizeType = sizeof(T);
        if(!canHold(sizeType))
        {
            if(_autoResize)
                increase_size(_heapSize * (4/3));
            else
            {
                Core::log::report(FATAL_ERROR, "Jam Allocator: the requested allocation is too large for the allocator, free up memory or increase the size of the allocator");
                return nullptr;
            }
        }

        T* ptr = nullptr;

        lockThreads(mutex);

        JamAllocator::flag finder;
        finder.size = sizeType;
        finder.offset = 0;
        BinarySearchTree<JamAllocator::flag*>* node = nullptr;
        if(_freeSpaces != nullptr)
        {
            if(_freeSpaces->has_data())
               node = _freeSpaces->find(&finder);
        }
        if(node != nullptr)
        {
            ptr = reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(_heap) + node->getData()->offset);
            if(!_usedSpaces->has_data())
                _usedSpaces = node;
            else
                _usedSpaces->add(node); // Give node to Used Spaces Tree
            _freeSpaces->remove(node, false);
        }
        if(ptr == nullptr) // If we haven't found free flag
        {
            JamAllocator::flag* flag = reinterpret_cast<JamAllocator::flag*>(reinterpret_cast<uintptr_t>(_heap) + _memUsed); // New flag
            flag->size = sizeType;
            _memUsed += sizeof(JamAllocator::flag);

            node = reinterpret_cast<BinarySearchTree<JamAllocator::flag*>*>(reinterpret_cast<uintptr_t>(_heap) + _memUsed); // New Node
            init_node(node, flag);
            flag->offset = _memUsed; // Set offset here to include node size

            if(_usedSpaces == nullptr || !_usedSpaces->has_data())
                _usedSpaces = node;
            else
                _usedSpaces->add(node); // Give node to Used Spaces Tree
            ptr = reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(_heap) + _memUsed); // Allocate Pointer
            _memUsed += sizeType;
        }

        unlockThreads(mutex);

        if(std::is_class<T>::value)
            new ((void*)ptr) T(std::forward<Args>(args)...);

    	return ptr;
    }

    template <typename T = void>
    void JamAllocator::free(T* ptr)
    {
        if(ptr == nullptr)
        {
            Core::log::report(WARNING, "Jam Allocator: you cannot free a nullptr");
            return;
        }
        if(_heap == nullptr)
        {
            Core::log::report(WARNING, "Jam Allocator: trying to free a pointer with an uninitialised allocator");
            return;
        }
        if(!contains((void*)ptr))
        {
            Core::log::report(WARNING, "Jam Allocator: a pointer allocated by another allocator is trying to be freed");
            return;
        }
        if(std::is_class<T>::value)
            ptr->~T();

        JamAllocator::flag* finder = nullptr;

        lockThreads(mutex);

        auto it = _usedSpaces->root_it();

        for(; it.hasNext(); it.next()) // flag finder
        {
            if(ptr == (void*)(reinterpret_cast<uintptr_t>(_heap) + it->getData()->offset))
                finder = it->getData();
        }

        bool oui = (void*)ptr == (void*)(finder + sizeof(JamAllocator::flag));
        std::cout << ptr << "   " << finder + sizeof(JamAllocator::flag) << "    match : " << oui << std::endl;

        if(finder != nullptr)
        {
            debugPrint(it->getData());
            _usedSpaces->remove(it.get_node(), false);
        }
        else
        {
            Core::log::report(ERROR, "JamAllocator : unable to find the flag of %p", ptr);
            unlockThreads(mutex);
            return;
        }
        
        debugPrint("test2");
        if(_freeSpaces == nullptr || !_freeSpaces->has_data())
        {
            debugPrint("test3");
            _freeSpaces = it.get_node();
        }
        else
        {
            debugPrint("test4");
            _freeSpaces->add(it.get_node());
        }
        unlockThreads(mutex);
        ptr = nullptr;
    }
}
