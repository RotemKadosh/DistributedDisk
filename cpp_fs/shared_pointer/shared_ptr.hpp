#ifndef ILRD_94_SHAREDPTR_HPP
#define ILRD_94_SHAREDPTR_HPP

#include "../utils/utils.hpp"


namespace ilrd
{

template <typename T>
T *Badptr()
{
    return reinterpret_cast<T *>(0xDEADBEEF);
}

template <typename T>
class SharedPtr
{
public:
    explicit SharedPtr(T *ptr_ = NULL); // pointer must point to memory allocated by
                                            //new (and not new[], malloc etc.),
                                            //otherwise undefined behavior
                                            //throws bad alloc

    ~SharedPtr() noexcept;
    SharedPtr(const SharedPtr<T>& other_);
    SharedPtr<T>& operator=(const SharedPtr<T>& other_);

    template <typename D>
    SharedPtr(const SharedPtr<D>& other_);    // conversion constructor
                                                // D should be derived from TS


    T& operator*() const; //The behavior is undefined if the stored pointer is null.
    T* operator->() const;

private:
    template <typename D>
    friend class SharedPtr;
    size_t *m_counter;
    T *m_ptr;
    void Use();
    void UnUse();
    
};

} // namespace ilrd

#endif // ILRD_94_SHAREDPTR_HPP