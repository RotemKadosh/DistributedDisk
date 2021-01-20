#ifndef ILRD_94_SHAREDPTR_IMPL_HPP
#define ILRD_94_SHAREDPTR_IMPL_HPP

#include "shared_ptr.hpp"
namespace ilrd
{
    template<typename T>
    SharedPtr<T>::SharedPtr(T *ptr_):
    m_counter(NULL),
    m_ptr(ptr_)
    {
        try
        {
            m_counter = new size_t(1);
        }
        catch(const std::bed_alloc& e)
        {
            free(m_ptr);
            m_ptr = Badptr();
            std::cerr << e.what() << '\n';
        } 
    }

    template<typename T>
    SharedPtr<T>::SharedPtr(const SharedPtr<T>& other_):
    m_counter(other_.m_counter),
    m_ptr(other_.m_ptr)
    {
        Use();
    }
    
    template<typename T>
    template <typename D>
    SharedPtr<T>::SharedPtr(const SharedPtr<D>& other_):
    m_counter(other_.m_counter),
    m_ptr(static_cast<T *>(other_.m_ptr))
    {
        Use();
    }

    template<typename T>
    SharedPtr<T>::~SharedPtr() noexcept
    {
        UnUse();
        if(0 == *m_counter)
        {
            delete m_ptr;
            m_ptr = Badptr<T>();
            delete m_counter;
            m_counter = Badptr<size_t>();
        }
    }


    template<typename T>
    SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other_)
    {

        SharedPtr<T> old (*this);
        UnUse();
        
        m_ptr = other_.m_ptr;
        m_counter = other_.m_counter;

        Use();
        
        return *this;
    }

    template<typename T>
    T& SharedPtr<T>::operator*() const
    {
        return *m_ptr;
    }

    template<typename T>
    T* SharedPtr<T>::operator->() const
    {
        return m_ptr;
    }

    template<typename T>
    inline void SharedPtr<T>::Use()
    {
        ++(*m_counter);
    }

    template<typename T>
    inline void SharedPtr<T>::UnUse()
    {
        --(*m_counter);
    }
} // namespace ilrd




#endif //ILRD_94_SHAREDPTR_IMPL_HPP