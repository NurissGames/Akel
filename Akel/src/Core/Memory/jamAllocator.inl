// This file is a part of Akel
// Authors : @kbz_8
// Created : 25/07/2021
// Updated : 12/11/2022

#include <Maths/maths.h>

namespace Ak
{
    void Error(std::string message, ...);
    void Warning(std::string message, ...);

    template <class T, typename ... Args>
    T* JamAllocator::alloc(Args&& ... args)
    {
		T* ptr = reinterpret_cast<T*>(internal_allocation(sizeof(T)));
        if constexpr(std::is_class<T>::value)
            ::new ((void*)ptr) T(std::forward<Args>(args)...);
    	return ptr;
    }

    template <class T>
    T* JamAllocator::alloc(size_t size)
    {
		return reinterpret_cast<T*>(internal_allocation(size));
    }

    template <class T>
    void JamAllocator::free(T* ptr)
    {
        if(ptr == nullptr)
        {
            Warning("Jam Allocator: you cannot free a nullptr");
            return;
        }
        if(_heap == nullptr)
        {
            Warning("Jam Allocator: trying to free a pointer with an uninitialised allocator");
            return;
        }
        if(!contains((void*)ptr))
        {
            Warning("Jam Allocator: a pointer allocated by another allocator is trying to be freed");
            return;
        }
        if constexpr(std::is_class<T>::value)
            ptr->~T();

		internal_free(ptr);
    }
}
