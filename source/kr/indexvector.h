#pragma once
#include <cstdint>
#include <stdexcept>

namespace kr
{

namespace _indexvec_detail
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
class indexvector
{
  private:
    using Storage_t = _indexvec_detail::storage_t<K, V>;

    Storage_t* m_data {nullptr};
    K* index {nullptr};

    K allocated {0};
    K used {0};
    K freeIndexes {0};

    inline K getFreeKey()
    {
        if (used >= allocated)
        {
            reserve(used * 2);
        }

        if (freeIndexes > 0)
        {
            K firstFree = index[allocated * 2 - freeIndexes];
            --freeIndexes;
            return firstFree;
        }
        else
        {
            return used;
        }
    }

  public:
    indexvector() = default;
    indexvector(const indexvector& rhs)
    {
        reserve(rhs.allocated);

        // copy the m_data
        for (K i = 0; i < rhs.used; ++i)
            new (&m_data[i].val) V((rhs.m_data[i].val));

        // Copy the indexes
        std::copy(rhs.index, rhs.index + 2 * rhs.allocated, index);

        used = rhs.used;
        freeIndexes = rhs.freeIndexes;
    }
    indexvector(indexvector&& rhs)
    {
        allocated = rhs.allocated;
        used = rhs.used;
        freeIndexes = rhs.freeIndexes;
        index = rhs.index;
        m_data = rhs.m_data;

        rhs.m_data = nullptr;
        rhs.index = nullptr;
        rhs.allocated = 0;
        rhs.used = 0;
        rhs.freeIndexes = 0;
    }
    indexvector& operator=(const indexvector& rhs)
    {
        // cleanup
        for (auto& i : *this)
            i.~V();
        used = 0;
        freeIndexes = 0;
        allocated = 0;
        delete[] index;
        delete[] m_data;
        index = nullptr;
        reserve(rhs.allocated);

        // copy the m_data/indexes
        for (K i = 0; i < rhs.used; ++i)
            new (&m_data[i].val) V((rhs.m_data[i].val));
        std::copy(rhs.index, rhs.index + 2 * rhs.allocated, index);

        // copy other values
        used = rhs.used;
        freeIndexes = rhs.freeIndexes;
        return *this;
    }
    indexvector& operator=(indexvector&& rhs)
    {
        // Cleanup
        for (auto& i : *this)
            i.~V();
        used = 0;
        freeIndexes = 0;
        allocated = 0;
        delete[] index;
        delete[] m_data;

        // steal
        allocated = rhs.allocated;
        used = rhs.used;
        freeIndexes = rhs.freeIndexes;
        index = rhs.index;
        m_data = rhs.m_data;

        // clean rhs
        rhs.m_data = nullptr;
        rhs.index = nullptr;
        rhs.allocated = 0;
        rhs.used = 0;
        rhs.freeIndexes = 0;
        return *this;
    }

    ~indexvector()
    {
        for (auto& i : *this)
            i.~V();
        delete[] m_data;
        delete[] index;
    }

    void clear()
    {
        for (auto& i : *this)
            i.~V();
        used = 0;
        freeIndexes = 0;
        for (auto it = index; it != (index + allocated * 2); ++it)
            *it = -1;
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
            auto olddata = m_data;
            index = new K[newsize * 2];
            m_data = new Storage_t[newsize];

            // move the m_data
            for (K i = 0; i < used; ++i)
                new (&m_data[i].val) V(std::move(olddata[i].val));

            // Clean indexes
            for (auto it = (index + allocated);
                 it != (index + newsize * 2 - freeIndexes); ++it)
            {
                *it = -1;
            }
            // Copy used part
            std::copy(&oldindex[0], &oldindex[allocated], &index[0]);
            // Copy free part
            std::copy(&oldindex[2 * allocated - freeIndexes],
                &oldindex[2 * allocated], &index[newsize * 2 - freeIndexes]);

            // Clean
            delete[] oldindex;
            delete[] olddata;

            allocated = newsize;
        }
        else
        {
            if (newsize <= 0)
                newsize = 32;
            index = new K[newsize * 2];
            m_data = new Storage_t[newsize];
            for (auto it = index; it != (index + newsize * 2); ++it)
                *it = -1;
            allocated = newsize;
        }
    }

    K capacity() const
    {
        return allocated;
    }

    V& push_back(V&& val)
    {
        auto freeid = getFreeKey();
        new (&m_data[used].val) V(std::move(val));
        m_data[used].id() = freeid;
        index[freeid] = used;
        used++;
        return m_data[used - 1].val;
    }

    V& push_back(const V& val)
    {
        auto freeid = getFreeKey();
        new (&m_data[used].val) V(val);
        m_data[used].id() = freeid;
        index[freeid] = used;
        used++;
        return m_data[used - 1].val;
    }

    template <class... Args>
    V& emplace_back(Args&&... args)
    {
        auto freeid = getFreeKey();
        new (&m_data[used].val) V((args)...);
        m_data[used].id() = freeid;
        index[freeid] = used;
        used++;
        return m_data[used - 1].val;
    }

    V& data(size_t key)
    {
        return m_data[key].val;
    }

    V& operator[](K key)
    {
        return m_data[index[key]].val;
    }

    const V& operator[](K key) const
    {
        return m_data[index[key]].val;
    }


    /// Throws exception when object does not exists
    V& at(K key)
    {
        if (key < 0 || key >= allocated || index[key] == -1)
            throw std::out_of_range {"out of range in inlineVec"};
        return m_data[index[key]].val;
    }

    /// Throws exception when object does not exists
    const V& at(K key) const
    {
        if (key < 0 || key >= allocated || index[key] == -1)
            throw std::out_of_range {"out of range in inlineVec"};
        return m_data[index[key]].val;
    }

    /// Throws exception when object does not exists
    bool has(K key) const
    {
        return not(key < 0 || key >= allocated || index[key] == -1);
    }

    void erase(K key)
    {
        auto pos = index[key];
        index[key] = -1; // mark index unused

        // add to free indexes stack
        freeIndexes++;
        index[2 * allocated - freeIndexes] = key;

        // Delete object
        used--;
        if (used != pos)
        {
            // Swap if not at the end of the vector
            index[m_data[used].id()] = pos;
            m_data[pos].val = std::move(m_data[used].val);
        }
        m_data[used].val.~V();
    }
    void erase(V* iter)
    {
        auto key = iter->id;
        // auto pos = index[key];
        auto pos = iter - begin();
        index[key] = -1; // mark index unused

        // add to free indexes stack
        freeIndexes++;
        index[2 * allocated - freeIndexes] = key;

        // Delete object
        used--;
        if (used != pos)
        {
            // Swap if not at the end of the vector
            index[m_data[used].id()] = pos;
            m_data[pos].val = std::move(m_data[used].val);
        }
        m_data[used].val.~V();
    }
    void erase(V* iter, V* iter2)
    {
        for (V* i = iter2-1; i >= iter; i--)
            erase(i);
    }

    V* begin()
    {
        return &m_data[0].val;
    }
    V* end()
    {
        return &m_data[used].val;
    }
    const V* begin() const
    {
        return &m_data[0].val;
    }
    const V* end() const
    {
        return &m_data[used].val;
    }

  private:
};

} /* kr */
