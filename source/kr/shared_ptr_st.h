#pragma once

namespace kr
{

/**
 * Single threaded version of std::share_ptr. Non-intrusive.
 */
template <class T>
class shared_ptr_st
{
  private:
    T* ptr;
    mutable unsigned* count;

  public:
    shared_ptr_st()
        : ptr(nullptr)
        , count(nullptr){};
    shared_ptr_st(T* target)
        : ptr(target)
        , count(nullptr){};
    shared_ptr_st(const shared_ptr_st& rhs)
        : ptr(rhs.ptr)
        , count(rhs.count)
    {
        if (!ptr)
            return;
        if (!count)
        {
            count = rhs.count = new unsigned(2);
        }
        else
            ++(*count);
    };
    shared_ptr_st(shared_ptr_st&& rhs)
        : ptr(rhs.ptr)
        , count(rhs.count)
    {
        rhs.ptr = nullptr;
        rhs.count = nullptr;
    };
    shared_ptr_st<T>& operator=(shared_ptr_st& rhs)
    {
        reset();
        ptr = rhs.ptr;
        count = rhs.count;
        if (!ptr)
            return *this;
        if (!count)
        {
            count = rhs.count = new unsigned(2);
        }
        else
        {
            ++(*count);
        }
        return *this;
    };
    shared_ptr_st<T>& operator=(shared_ptr_st&& rhs)
    {
        reset();

        ptr = rhs.ptr;
        count = rhs.count;

        rhs.ptr = nullptr;
        rhs.count = nullptr;
        return *this;
    };

    void reset()
    {
        if (!count)
        {
            delete ptr;
        }
        else if (*count == 1)
        {
            delete ptr;
            delete count;
        }
        else
        {
            --(*count);
        }
        ptr = nullptr;
        count = nullptr;
    }

    ~shared_ptr_st()
    {
        reset();
    }

    T& operator*()
    {
        return *ptr;
    }
    T* operator->()
    {
        return ptr;
    }

    explicit operator bool() const
    {
        return ptr != nullptr;
    }
    bool unique() const
    {
        return use_count() == 1;
    }
    unsigned use_count() const
    {
        if (ptr)
        {
            if (count == nullptr)
                return 1;
            else
                return *count;
        }
        else
            return 0;
    }
    void swap(kr::shared_ptr_st<T>& rhs)
    {
        T* _ptr = ptr;
        unsigned* _count = count;

        ptr = rhs.ptr;
        count = rhs.count;

        rhs.ptr = _ptr;
        rhs.count = _count;
    }
};

template <class T, class... Args>
shared_ptr_st<T> make_shared_st(Args&&... args)
{
    return shared_ptr_st<T>(new T((args)...));
}

} /* kr */

namespace std
{

template <class T>
void swap(kr::shared_ptr_st<T>& lhs, kr::shared_ptr_st<T>& rhs)
{
    lhs.swap(rhs);
}
} /* std */
