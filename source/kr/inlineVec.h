#pragma once
#include <cstdint>
#include <stdexcept>

namespace kr
{

namespace _inlineVec_detail
{
    template <class T>
    union storage_t
    {
        unsigned char dummy_;
        T val;

        constexpr storage_t() noexcept
            : dummy_() {};

        constexpr storage_t(T& other)
            : val(other) {};

        constexpr storage_t(T&& other)
            : val(other) {};

        template <class... Args>
        constexpr storage_t(Args&&... args)
            : val((args)...)
        {
        }

        ~storage_t() {};
    };

} /* _inlineVec_detail */

template <class T, int max_vector_size, class SIZE_T=std::size_t>
class inlineVec
{
  private:
    SIZE_T count {0};
    _inlineVec_detail::storage_t<T> data[max_vector_size];


  public:
    inlineVec() = default;
    inlineVec(const inlineVec<T, max_vector_size>& rhs)
    {
        for (const auto& it : rhs)
            this->push_back(it);
    }
    inlineVec(inlineVec<T, max_vector_size>&& rhs)
    {
        for (auto& it : rhs)
            this->push_back(std::move(it));
        rhs.count = 0;
    }

    inlineVec& operator=(const inlineVec<T, max_vector_size>& rhs)
    {
        if (&rhs == this)
            return *this;

        this->clear();
        for (const auto& it : rhs)
            this->push_back(it);
        return *this;
    }
    inlineVec<T, max_vector_size>& operator=(
        inlineVec<T, max_vector_size>&& rhs) noexcept
    {
        if (&rhs == this)
            return *this;
        this->clear();
        for (auto& it : rhs)
            this->push_back(std::move(it));
        rhs.count = 0;
        return *this;
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
        new (&data[count].val) T(std::move(rhs));
        count++;
    }
    void push_back(const T& rhs)
    {
        new (&data[count].val) T(rhs);
        count++;
    }
    template <class... Args>
    void emplace_back(Args&&... args)
    {
        new (&data[count].val) T((args)...);
        count++;
    }
    void pop_back()
    {
        count--;
        data[count].val.~T();
    }
    T& operator[](SIZE_T index)
    {
        return data[index].val;
    }
    const T& operator[](SIZE_T index) const
    {
        return data[index].val;
    }
    T& at(SIZE_T index)
    {
        if (index >= count)
            throw std::out_of_range {"out of range in inlineVec"};
        return data[index].val;
    }
    const T& at(SIZE_T index) const
    {
        if (index >= count)
            throw std::out_of_range {"out of range in inlineVec"};
        return data[index].val;
    }
    const T& front() const
    {
        return data[0].val;
    }
    const T& back() const
    {
        return data[count - 1].val;
    }
    T& front()
    {
        return data[0].val;
    }
    T& back()
    {
        return data[count - 1].val;
    }
    size_t max_size() const
    {
        return max_vector_size;
    }
    size_t capacity() const
    {
        return max_vector_size;
    }
    bool full() const
    {
        return count >= max_vector_size;
    }
    size_t size() const
    {
        return count;
    }
    bool empty() const
    {
        return count == 0;
    }
    void erase(int pos)
    {
        data[pos].val.~T();
        std::move((T*)&(data[pos + 1]), (T*)&data[count].val, (T*)&(data[pos]));
        count--;
    }
    T* find(const T& value) const
    {
        for (const auto& it : *this)
        {
            if(it == value)
                return *it;
        }
        return nullptr;
    }
    bool try_add(const T& value)
    {
        for (const auto& it : *this)
        {
            if(it == value)
                return false;
        }
        push_back(value);
        return true;
    }
    T* insert(int pos, const T& value)
    {
        if (count == 0)
        {
            new (&data[pos].val) T(value);
            count++;
            return begin();
        }
        new (&data[count].val) T(std::move(data[count - 1].val));
        std::move_backward(
            (T*)&(data[pos]), (T*)&data[count - 1].val, (T*)&(data[count]));
        count++;
        // vector was size 0
        data[pos].val = value;

        return (T*)&(data[pos].val);
    }
    T* insert(int pos, T&& value)
    {
        if (count == 0)
        {
            new (&data[pos].val) T(std::move(value));
            count++;
            return begin();
        }
        new (&data[count].val) T(std::move(data[count - 1].val));
        std::move_backward(
            (T*)&(data[pos]), (T*)&data[count - 1].val, (T*)&(data[count]));
        count++;
        // vector was size 0
        data[pos].val = std::move(value);

        return (T*)&(data[pos].val);
    }
    // new (&data[count].val) T((args)...);
    template <class... Args>
    T* insert(int pos, Args&&... args)
    {
        if (count == 0)
        {
            new (&data[pos].val) T((args)...);
            count++;
            return begin();
        }
        new (&data[count].val) T(std::move(data[count - 1].val));
        std::move_backward(
            (T*)&(data[pos]), (T*)&data[count - 1].val, (T*)&(data[count]));
        count++;
        // vector was size 0
        new (&data[pos].val) T((args)...);
        return (T*)&(data[pos].val);
    }


    T* begin()
    {
        return &data[0].val;
    }
    T* end()
    {
        return &data[count].val;
    }
    const T* begin() const
    {
        return &data[0].val;
    }
    const T* end() const
    {
        return &data[count].val;
    }
};

} /* kr */
