#pragma once
#include <cstdint>
#include <stdexcept>

namespace kr
{

namespace _flatimap_detail
{
    // TODO make it possible even for simple things
    template <class K, class T>
    union storage_t
    {
        unsigned char dummy_;
        T val;

        constexpr storage_t() noexcept
            : dummy_() {};

        constexpr storage_t(T& other, K newid)
            : val(other)
        {
            val.id = newid;
        };

        constexpr storage_t(T&& other)
            : val(other) {};

        template <class... Args>
        constexpr storage_t(Args&&... args)
            : val((args)...)
        {
        }

        K& id()
        {
            return val.id;
        }

        ~storage_t() {};
    };

} /* _indexvec_detail */


template <class K, class V>
class flatimap
{
  private:
    using Storage_t = _flatimap_detail::storage_t<K, V>;

    Storage_t* data {nullptr};
    struct INDEX
    {
        K key;
        K index;
    };
    INDEX* index {nullptr};

    K allocated {0};
    K used {0};


    /// Used to always get a position where to insert key
    K get_mid(K key) const
    {
        // early exits
        if (used == 0)
            return 0;
        if (index[used - 1].key < key)
            return used;

        K l = 0;
        K r = used - 1;

        while (l <= r)
        {
            K mid = (r + l) / 2;
            if (index[mid].key < key)
                l = mid + 1;
            else if (index[mid].key > key)
                r = mid - 1;
            else
                return mid;
        }
        return l;
    }

    K get_index_to_index(K key) const
    {

        if (used == 0)
            return -1;
        K l = 0;
        K r = used - 1;

        while (l <= r)
        {
            K mid = (r + l) / 2;
            if (index[mid].key < key)
                l = mid + 1;
            else if (index[mid].key > key)
                r = mid - 1;
            else
                return mid;
        }
        return -1;
    }

    K get(K key) const
    {
        auto i = get_index_to_index(key);
        if (i == -1)
            return -1;
        return index[i].index;
    }


  public:
    flatimap() = default;
    // TODO constructors
    ~flatimap()
    {
        for (auto& i : *this)
            i.~V();
        delete[] data;
        delete[] index;
    }

    void clear()
    {
        for (auto& i : *this)
            i.~V();
        used = 0;
    }

    K size() const
    {
        return used;
    }


    bool empty() const
    {
        return used == 0;
    }

    void reserve(K newsize)
    {
        if (index != nullptr)
        {
            // Allocate new space
            auto oldindex = index;
            auto olddata = data;
            index = new INDEX[newsize];
            data = new Storage_t[newsize];

            // move the data
            for (K i = 0; i < used; ++i)
                new (&data[i].val) V(std::move(olddata[i].val));
            std::copy(&oldindex[0], &oldindex[allocated], &index[0]);

            // Clean
            delete[] oldindex;
            delete[] olddata;

            allocated = newsize;
        }
        else
        {
            if (newsize <= 0)
                newsize = 16;
            index = new INDEX[newsize];
            data = new Storage_t[newsize];
            allocated = newsize;
        }
    }

    K capacity() const
    {
        return allocated;
    }

    bool contains(K key) const
    {
        return (get(key) != -1);
    }

    V& operator[](K key)
    {
        return data[get(key)].val;
    }

    /// Throws exception when object does not exists
    V& at(K key)
    {
        auto id = get(key);
        if (id == -1)
            throw std::out_of_range {"out of range in flatimap"};
        else
            return data[id].val;
    }

    V& put(K key, V&& value)
    {
        if (used >= (allocated - 1))
            reserve(allocated * 2);
        auto in = get_mid(key);
        std::copy_backward(&(index[in]), &(index[used]), &(index[used + 1]));
        index[in].key = key;
        index[in].index = used;
        new (&data[used].val) V(std::move(value));
        used++;
        return data[used - 1].val;
    }
    V& insert_or_assign(K key, V&& value)
    {
        return put(key, value);
    }
    V& put(K key, const V& value)
    {
        if (used >= (allocated - 1))
            reserve(allocated * 2);
        auto in = get_mid(key);
        std::copy_backward(&(index[in]), &(index[used]), &(index[used + 1]));
        index[in].key = key;
        index[in].index = used;
        new (&data[used].val) V(value);
        used++;
        return data[used - 1].val;
    }
    V& insert_or_assign(K key, const V& value)
    {
        return put(key, value);
    }
    template <class... Args>
    V& emplace(K key, Args&&... args)
    {
        if (used >= (allocated - 1))
            reserve(allocated * 2);
        auto in = get_mid(key);
        std::copy_backward(&(index[in]), &(index[used]), &(index[used + 1]));
        index[in].key = key;
        index[in].index = used;
        new (&data[used].val) V((args)...);
        used++;
        return data[used - 1].val;
    }

    void erase(K key)
    {
        auto pos_index = get_index_to_index(key);
        auto pos = index[pos_index].index;

        // delete from index
        std::copy(&(index[pos_index + 1]), &(index[used]), &(index[pos_index]));

        // Delete object
        used--;
        if (used != pos)
        {
            // Swap if not at the end of the vector
            index[data[used].id()].index = pos;
            data[pos].val = std::move(data[used].val);
        }
        data[used].val.~V();
    }

    V* begin()
    {
        return &data[0].val;
    }
    V* end()
    {
        return &data[used].val;
    }
    const V* begin() const
    {
        return &data[0].val;
    }
    const V* end() const
    {
        return &data[used].val;
    }

  private:
};

} /* kr */
