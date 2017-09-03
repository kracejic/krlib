
#pragma once
#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace kr
{

namespace _inlineRing_detail
{
    template <class T>
    union storage_t {
        unsigned char dummy_;
        T val;

        constexpr storage_t() noexcept
            : dummy_(){};

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

    constexpr bool is_powerof2(int v)
    {
        return v && ((v & (v - 1)) == 0);
    }

} /* _inlineRing_detail */

template <class T, int max_ring_size>
class inlineRing
{
  private:
    uint32_t mFront{0};
    uint32_t mBack{0};
    _inlineRing_detail::storage_t<T> data[max_ring_size];

    uint32_t addr(uint32_t index)
    {
        return index & (max_ring_size - 1);
    }

    static_assert(_inlineRing_detail::is_powerof2(max_ring_size),
        "size must be power of two");

  public:
    inlineRing() = default;
    inlineRing(const inlineRing<T, max_ring_size>& rhs)
    {
        for (const auto& it : rhs)
            this->push_back(it);
    }
    inlineRing(inlineRing<T, max_ring_size>&& rhs)
    {
        for (auto& it : rhs)
            this->push_back(std::move(it));
        rhs.count = 0;
    }

    ~inlineRing()
    {
        for (auto& x : *this)
            x.~T();
    }
    void clear()
    {
        for (auto& x : *this)
            x.~T();
        mFront = 0;
        mBack = 0;
    }

    void push_front(T&& rhs)
    {
        mFront--;
        new (&data[addr(mFront)].val) T(std::move(rhs));
    }
    void push_front(const T& rhs)
    {
        mFront--;
        new (&data[addr(mFront)].val) T(rhs);
    }
    template <class... Args>
    void emplace_front(Args&&... args)
    {
        mFront--;
        new (&data[addr(mFront)].val) T((args)...);
    }

    void push_back(T&& rhs)
    {
        new (&data[addr(mBack)].val) T(std::move(rhs));
        mBack++;
    }
    void push_back(const T& rhs)
    {
        new (&data[addr(mBack)].val) T(rhs);
        mBack++;
    }
    template <class... Args>
    void emplace_back(Args&&... args)
    {
        new (&data[addr(mBack)].val) T((args)...);
        mBack++;
    }

    void pop_front()
    {
        mFront++;
        data[addr(mFront)].val.~T();
    }
    void pop_back()
    {
        mBack--;
        data[addr(mBack - 1)].val.~T();
    }

    T& front()
    {
        return data[addr(mFront)].val;
    }
    T& back()
    {
        return data[addr(mBack - 1)].val;
    }

    T& operator[](uint32_t index)
    {
        return data[addr(mFront + index)].val;
    }
    T& at(std::size_t index)
    {
        if (index >= (mBack - mFront))
            throw std::out_of_range{"out of range in inlineRing"};
        return data[addr(mFront + index)].val;
    }
    size_t max_size()
    {
        return max_ring_size;
    }
    size_t capacity()
    {
        return max_ring_size;
    }
    size_t size()
    {
        return mBack - mFront;
    }
    bool empty()
    {
        return mBack == mFront;
    }

    // void erase(int pos)
    // {
    //     data[pos].val.~T();
    //     std::move((T*)&(data[pos + 1]), (T*)&data[count].val,
    //     (T*)&(data[pos]));
    //     count--;
    // }
    // T* insert(int pos, const T& value)
    // {
    //     if (count == 0)
    //     {
    //         new (&data[pos].val) T(value);
    //         count++;
    //         return begin();
    //     }
    //     new (&data[count].val) T(std::move(data[count - 1].val));
    //     std::move_backward(
    //         (T*)&(data[pos]), (T*)&data[count - 1].val, (T*)&(data[count]));
    //     count++;
    //     // vector was size 0
    //     data[pos].val = std::move(value);
    //
    //     return (T*)&(data[pos].val);
    // }
    // T* insert(int pos, T&& value)
    // {
    //     if (count == 0)
    //     {
    //         new (&data[pos].val) T(std::move(value));
    //         count++;
    //         return begin();
    //     }
    //     new (&data[count].val) T(std::move(data[count - 1].val));
    //     std::move_backward(
    //         (T*)&(data[pos]), (T*)&data[count - 1].val, (T*)&(data[count]));
    //     count++;
    //     // vector was size 0
    //     data[pos].val = std::move(value);
    //
    //     return (T*)&(data[pos].val);
    // }
    // // new (&data[count].val) T((args)...);
    // template <class... Args>
    // T* insert(int pos, Args&&... args)
    // {
    //     if (count == 0)
    //     {
    //         new (&data[pos].val) T((args)...);
    //         count++;
    //         return begin();
    //     }
    //     new (&data[count].val) T(std::move(data[count - 1].val));
    //     std::move_backward(
    //         (T*)&(data[pos]), (T*)&data[count - 1].val, (T*)&(data[count]));
    //     count++;
    //     // vector was size 0
    //     new (&data[pos].val) T((args)...);
    //     return (T*)&(data[pos].val);
    // }
    //
    //

    class InlineRingIterator
    {
      private:
        uint32_t pos;
        inlineRing<T, max_ring_size>& parent;

      public:
        InlineRingIterator(inlineRing<T, max_ring_size>& _parent, uint32_t _pos)
            : parent(_parent)
            , pos(_pos){};
        InlineRingIterator& operator--()
        {
            pos--;
            return *this;
        }
        InlineRingIterator& operator++()
        {
            pos++;
            return *this;
        }
        bool operator!=(const InlineRingIterator& rhs)
        {
            return pos != rhs.pos;
        }
        T& operator*()
        {
            return parent.data[parent.addr(pos)].val;
        }
    };
    InlineRingIterator begin()
    {
        return InlineRingIterator(*this, mFront);
    }
    InlineRingIterator end()
    {
        return InlineRingIterator(*this, mBack);
    }
};

} /* kr */
