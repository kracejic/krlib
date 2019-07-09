#pragma once
#include <cstdio>
#include <cstring>
#include <utility>
#include <type_traits>

namespace kr
{
    

template<class T, size_t PADDING>
class paddedVector
{
public:
    void reserve(size_t newAllocated)
    {
        if (newAllocated <= count)
            return;

        auto newdata = new STORAGE[newAllocated];

        // Do we need to move the old data?
        if (data != nullptr)
        {
            std::memcpy(newdata, data, count*sizeof(STORAGE));
            delete[] data;
        }

        data = newdata;
    }

    size_t size() {
        return count;
    }

    size_t element_size()
    {
        return sizeof(STORAGE);
    }

    void clear()
    {
        for (size_t i = 0; i < count; ++i)
            ((T*)(&data[i]))->~T();
        count = 0;
    }

    template <class... Args>
    void emplace_back(Args&&... args)
    {
        if(count == allocated)
            reserve(8 + (allocated*4)/3);

        new (&(data[count])) T((args)...);
        count++;
    }

    void push_back(T&& rhs)
    {
        if(count == allocated)
            reserve(8 + (allocated*4)/3);

        new (&(data[count])) T(std::move(rhs));
        count++;
    }

    T& operator[](size_t index)
    {
        return data[index];
    }



private:
    using STORAGE = std::aligned_storage_t<sizeof(T)+PADDING>;
    STORAGE *data = nullptr;
    size_t allocated = 0;
    size_t count = 0;

};

} /* kr */ 
