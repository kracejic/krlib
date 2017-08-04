#pragma once
#include <cstdint>
#include <stdexcept>

namespace kr
{

namespace _inlineVec_detail
{
    template <class T>
    union storage_t {
        unsigned char dummy_;
        T val;

        constexpr storage_t() noexcept : dummy_(){};

        constexpr storage_t(T& other)
            : val(other){};

        constexpr storage_t(T&& other)
            : val(other){};

        template <class... Args>
        constexpr storage_t(Args&&... args)
            : val((args)...)
        {
        }

        ~storage_t(){};
    };

} /* _inlineVec_detail */

template <class T, int max_vector_size>
class inlineVec
{
  private:
    std::size_t count{0};
    _inlineVec_detail::storage_t<T> data[max_vector_size];


  public:
    inlineVec() = default;
    inlineVec(const inlineVec<T, max_vector_size>& rhs)
    {
        for(const auto& it : rhs)
            this->push_back(it);
    }
    inlineVec(inlineVec<T, max_vector_size>&& rhs)
    {
        for(const auto& it : rhs)
            this->push_back(std::move(it));
        rhs.count = 0;
    }

    ~inlineVec()
    {
        for (size_t i = 0; i < count; ++i)
            data[i].val.~T();
    }
    void clear()
    {
        for (size_t i = 0; i < count; ++i)
            data[i].val.~T();
        count = 0;
    }
    void push_back(T&& rhs)
    {
        // TODO assert
        // assert(count < max_vector_size);
        new (&data[count].val) T(std::move(rhs));
        count++;
    }
    void push_back(const T& rhs)
    {
        // TODO assert
        // assert(count < max_vector_size);
        new (&data[count].val) T(rhs);
        count++;
    }
    template <class... Args>
    void emplace_back(Args&&... args)
    {
        // TODO assert
        // assert(count < max_vector_size);
        new (&data[count].val) T((args)...);
        count++;
    }
    void pop_back()
    {
        // TODO assert
        // assert(count > 0);
        count--;
        data[count].val.~T();
    }
    T& operator[](std::size_t index)
    {
        return data[index].val;
    }
    T& at(std::size_t index)
    {
        if (index >= count)
            throw std::out_of_range{"out of range in inlineVec"};
        return data[index].val;
    }
    T& front()
    {
        // TODO assert
        // assert(count > 0);
        return data[0].val;
    }
    T& back()
    {
        // TODO assert
        // assert(count > 0);
        return data[count - 1].val;
    }
    size_t max_size()
    {
        return max_vector_size;
    }
    size_t capacity()
    {
        return max_vector_size;
    }
    size_t size()
    {
        return count;
    }
    bool empty()
    {
        return count == 0;
    }


    T* begin()
    {
        return &data[0].val;
    }
    T* end()
    {
        return &data[count + 1].val;
    }
};

} /* kr */
