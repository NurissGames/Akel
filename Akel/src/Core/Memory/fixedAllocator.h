// This file is a part of Akel
// CREATED : 18/07/2021
// UPDATED : 23/11/2021

#ifndef __AK_FIXED_ALLOCATOR__
#define __AK_FIXED_ALLOCATOR__

#include <Akpch.h>
#include <Utils/utils.h>

namespace Ak
{
    class FixedAllocator
    {
        public:
            FixedAllocator() = default;

            void init(size_t blockSize, size_t numBlocks);
            void resize(size_t numBlocks);
            bool canAlloc();
            bool contains(void* ptr) const;
            void autoResize(bool set);
            void destroy();
            inline bool is_init() const { return _heap != nullptr; }

            template <typename T = void, typename ... Args>
            T* alloc(Args&& ... args);

            template <typename T = void>
            void free(T* ptr);

            ~FixedAllocator();

        private:
            size_t _block_size = 0;
            void* _heap = nullptr;
        	size_t _heap_size = 0;
            std::vector<bool> _bits;
            bool _autoResize = false;
            std::vector<bool>::reverse_iterator _it;
            inline static MutexHandel mutex;
            int _allocator_number = 0;
    };
}

#include <Core/Memory/fixedAllocator.inl>

#endif // __AK_FIXED_ALLOCATOR__
