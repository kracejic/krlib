
#pragma once
#include <cstdint>
#include <stdexcept>
#include <type_traits>

#include <algorithm>

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


template <class T, int max_ring_size, typename pointerType>
class inlineRing
{
  private:
    pointerType mFront{0};
    pointerType mBack{0};
    _inlineRing_detail::storage_t<T> data[max_ring_size];

    pointerType addr(pointerType index)
    {
        return index & (max_ring_size - 1);
    }

    static_assert(_inlineRing_detail::is_powerof2(max_ring_size),
        "size must be power of two");

  public:
    inlineRing() = default;
    inlineRing(const inlineRing<T, max_ring_size, pointerType>& rhs)
    {
        for (const auto& it : rhs)
            this->push_back(it);
    }
    inlineRing(inlineRing<T, max_ring_size, pointerType>&& rhs)
    {
        for (auto& it : rhs)
            this->push_back(std::move(it));
        rhs.mFront = 0;
        rhs.mBack = 0;
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

    T& operator[](pointerType index)
    {
        return data[addr(mFront + index)].val;
    }
    T& at(pointerType index)
    {
        if (index >= (mBack - mFront))
            throw std::out_of_range{"out of range in inlineRing"};
        return data[addr(mFront + index)].val;
    }
    pointerType max_size()
    {
        return max_ring_size;
    }
    pointerType capacity()
    {
        return max_ring_size;
    }
    pointerType size()
    {
        return mBack - mFront;
    }
    bool full()
    {
        return size() >= capacity();
    }
    bool empty()
    {
        return mBack == mFront;
    }

    void erase(int pos)
    {
        data[addr(pos)].val.~T();
        std::move(iterator(*this, mFront + pos + 1), iterator(*this, mBack),
            iterator(*this, mFront + pos));
        mBack--;
    }

    class iterator
    {
      private:
        inlineRing<T, max_ring_size, pointerType>& parent;
        pointerType pos;

      public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef T difference_type;
        typedef T* pointer;
        typedef T& reference;


        // typedef T value_type;
        iterator(inlineRing<T, max_ring_size, pointerType>& _parent,
            pointerType _pos)
            : parent(_parent)
            , pos(_pos){};
        iterator& operator--()
        {
            pos--;
            return *this;
        }
        iterator& operator++()
        {
            pos++;
            return *this;
        }
        bool operator!=(const iterator& rhs)
        {
            return pos != rhs.pos;
        }
        T& operator*()
        {
            return parent.data[parent.addr(pos)].val;
        }
    };

    iterator begin()
    {
        return iterator(*this, mFront);
    }
    iterator end()
    {
        return iterator(*this, mBack);
    }
    iterator insert(int pos, const T& value)
    {
        if (mFront == mBack)
        {
            new (&data[addr(mFront + pos)].val) T(value);
            mBack++;
            return begin();
        }
        new (&data[addr(mBack)].val) T(std::move(data[addr(mBack - 1)].val));
        std::move_backward(iterator(*this, mFront + pos),
            iterator(*this, mBack - 1), iterator(*this, mBack));
        mBack++;
        data[addr(mFront + pos)].val = value;

        return iterator(*this, pos);
    }
    iterator insert(int pos, T&& value)
    {
        if (mFront == mBack)
        {
            new (&data[addr(mFront + pos)].val) T(std::move(value));
            mBack++;
            return begin();
        }
        new (&data[addr(mBack)].val) T(std::move(data[addr(mBack - 1)].val));
        std::move_backward(iterator(*this, mFront + pos),
            iterator(*this, mBack - 1), iterator(*this, mBack));
        mBack++;
        data[addr(mFront + pos)].val = std::move(value);

        return iterator(*this, pos);
    }
    template <class... Args>
    iterator insert(int pos, Args&&... args)
    {
        if (mFront == mBack)
        {
            new (&data[addr(mFront + pos)].val) T((args)...);
            mBack++;
            return begin();
        }
        new (&data[addr(mBack)].val) T(std::move(data[addr(mBack - 1)].val));
        std::move_backward(iterator(*this, mFront + pos),
            iterator(*this, mBack - 1), iterator(*this, mBack));
        mBack++;
        new (&data[addr(mFront + pos)].val) T((args)...);

        return iterator(*this, pos);
    }
};


template <class T, int max_ring_size>
using inlineRing8 = inlineRing<T, max_ring_size, uint8_t>;

template <class T, int max_ring_size>
using inlineRing16 = inlineRing<T, max_ring_size, uint16_t>;

template <class T, int max_ring_size>
using inlineRing32 = inlineRing<T, max_ring_size, uint32_t>;

template <class T, int max_ring_size>
using inlineRing64 = inlineRing<T, max_ring_size, uint64_t>;

} /* kr */
