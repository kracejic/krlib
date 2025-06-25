#include <stdexcept>
#include <utility>

namespace kr
{
namespace _optional_detail
{
    template <class T>
    union storage_t {
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

} /* _optional_detail */
template <typename T>
class optional
{
  private:
    _optional_detail::storage_t<T> storage;
    bool initialized;


  public:
    optional()
        : initialized(false) {};

    optional(const T& other)
        : storage(other)
        , initialized(true) {};

    optional(T&& other)
        : storage(other)
        , initialized(true) {};

    optional(optional<T>& other)
        : initialized(other.initialized)
    {
        if (other.initialized)
            storage.val = other.storage.val;
    };
    optional(optional<T>&& other)
        : initialized(other.initialized)
    {
        if (other.initialized)
            storage.val = other.storage.val;
    };
    // TODO support copy/move assigment (even with replacing)

    template <class... Args>
    optional(Args&&... args)
        : storage((args)...)
        , initialized(true)
    {
    }

    ~optional()
    {
        if (initialized)
            storage.val.T::~T();
    }


    constexpr explicit operator bool()
    {
        return initialized;
    };


    constexpr bool has_value()
    {
        return initialized;
    };

    constexpr const T* operator->() const
    {
        return &storage.val;
    };
    constexpr T* operator->()
    {
        return &storage.val;
    };
    constexpr const T& operator*() const&
    {
        return storage.val;
    };
    constexpr T& operator*() &
    {
        return storage.val;
    };
    constexpr const T&& operator*() const&&
    {
        return storage.val;
    };
    constexpr T&& operator*() &&
    {
        return storage.val;
    };


    constexpr T& value()
    {
        if (not initialized)
            throw std::range_error("bad optional");
        return storage.val;
    }
    constexpr T& value_or(T& other)
    {
        if (not initialized)
            return other;
        return storage.val;
    }
    constexpr const T& value_or(const T& other) const
    {
        if (not initialized)
            return other;
        return storage.val;
    }
};
} /* kr */
