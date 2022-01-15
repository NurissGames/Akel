// This file is a part of Akel
// CREATED : 17/11/2021
// UPDATED : 15/01/2022

#ifndef __AK_BINARY_SEARCH_TREE__
#define __AK_BINARY_SEARCH_TREE__

#include <Core/core.h>

namespace Ak
{
    void Error(std::string message, ...);
    void Warning(std::string message, ...);

    template <typename T>
    class BinarySearchTree
    {
        public:
            struct iterator
            {
                using pointer = BinarySearchTree<T>*;
                using reference = BinarySearchTree<T>&;

                void fillStack(pointer node)
                {
                    while(node != nullptr)
                    {
                        st.push(node);
                        node = node->getLeft();
                    }
                }
                
                explicit iterator(pointer root) { fillStack(root); }

                reference next()
                {
                    pointer curr = st.top();
                    st.pop();
                    if(curr->getRight())
                        fillStack(curr->getRight());
                    return *curr;
                }

                inline reference forward_next() { return *st.top(); }

                inline bool hasNext() { return !st.empty(); }

                inline reference operator*() const
                {
                    if(!st.empty())
                        return *st.top();
                    else
                        Error("Cannot return the top of an empty stack");
                }
                inline pointer operator->()
                {
                    if(!st.empty())
                        return st.top();
                    else
                        Error("Cannot return the top of an empty stack");
                }
                inline pointer get_node()
                {
                    if(!st.empty())
                        return st.top();
                    else
                        Error("Cannot return the top of an empty stack");
                }

                inline friend bool operator== (const iterator& a, const iterator& b) { return a.st.top() == b.st.top(); }
                inline friend bool operator!= (const iterator& a, const iterator& b) { return a.st.top() != b.st.top(); }

                // Prefix increment
                inline iterator& operator++() { next(); return *this; }  
                // Postfix increment
                inline iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
                
                private:
                    std::stack<pointer> st;
            };

            explicit BinarySearchTree() = delete;
            explicit BinarySearchTree(T&& data);
            void add(T&& data);
            void add(BinarySearchTree<T>* node);
            void remove(T&& data);
            void remove(BinarySearchTree<T>* node, bool free = true);
            BinarySearchTree<T>* find_minimum();
            BinarySearchTree<T>* find(T&& data);
            BinarySearchTree<T>* find_parent(T&& data);
            BinarySearchTree<T>* find_parent(BinarySearchTree<T>* node);
            inline BinarySearchTree<T>* getRight() { return _right; }
            inline BinarySearchTree<T>* getLeft() { return _left; }
            inline T& getData() { return _data; }
            inline T getData_copy() { return _data; }
            inline void setData(T&& data) { _data = std::forward<T>(data); }
            inline void setRight(BinarySearchTree<T>* right) { _right = right; }
            inline void setLeft(BinarySearchTree<T>* left) { _left = left; }
            inline bool empty() const noexcept { return _right == nullptr && _left == nullptr && !_data; }
            inline bool has_data() const noexcept { return is_init; }

            inline void set_greater_operation(bool(*greater)(T, T)) noexcept { _greater = greater; }
            inline void set_less_operation(bool(*less)(T, T)) noexcept { _less = less; }
            inline void set_equal_operation(bool(*equal)(T, T)) noexcept { _equal = equal; }

            inline iterator root_it() { return iterator(this); }

            ~BinarySearchTree();

        private:
            T _data;
            bool is_init = false;
            
            // Using function pointers and not func::function or std::function due to their size (32 bytes)
            bool (*_greater)(T, T) = [](T new_data, T current_data){ return new_data  > current_data; };
            bool (*_equal)(T, T)   = [](T new_data, T current_data){ return new_data == current_data; };
            bool (*_less)(T, T)    = [](T new_data, T current_data){ return new_data  < current_data; };

            BinarySearchTree<T>* _left = nullptr;
            BinarySearchTree<T>* _right = nullptr;
    };
}

#include <Utils/Containers/bst.inl>

#endif // __AK_BINARY_SEARCH_TREE__
