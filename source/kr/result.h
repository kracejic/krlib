#pragma once

#include <stdexcept>
#include <string>
#include <utility>
#include <type_traits>

namespace kr
{

namespace _result_detail
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

        constexpr void erase()
        {
            val.T::~T();
        }

        constexpr T* valptr()
        {
            return &val;
        }
        constexpr T& valref()
        {
            return val;
        }
        constexpr const T* valptr() const
        {
            return &val;
        }
        constexpr const T& valref() const
        {
            return val;
        }

        ~storage_t() {};
    };

    template <class T>
    union storage_t<T&> {
        unsigned char dummy_;
        T* val;

        constexpr storage_t() noexcept
            : dummy_() {};

        constexpr storage_t(T& other)
            : val(&other) {};

        constexpr storage_t(T&& other) = delete;

        template <class... Args>
        constexpr storage_t(Args&&... args)
            : val((args)...)
        {
        }

        constexpr void erase() {}

        constexpr T* valptr()
        {
            return val;
        }
        constexpr T& valref()
        {
            return *val;
        }
        constexpr const T* valptr() const
        {
            return val;
        }
        constexpr const T& valref() const
        {
            return *val;
        }


        ~storage_t() {};
    };

} /* _result_detail */

enum whatever
{
    FAILED
};


template <class T>
class result
{
  private:
    _result_detail::storage_t<T> storage;
    bool _success = false;
    std::string _error = "";

  public:
    result(whatever f)
        : _success(false)
    {
        (void)(f);
    };
    result(whatever f, const std::string& error)
        : _success(false)
        , _error(error)
    {
        (void)(f);
    };
    result(whatever f, const char* error)
        : _success(false)
        , _error(error)
    {
        (void)(f);
    };

    result()
        : _success(false) {};

    result(const T& other)
        : storage(other)
        , _success(true) {};

    template<typename = std::enable_if<std::is_reference<T>::value>>
    result(T&& other)
        : storage(other)
        , _success(true) {}

    result(result<T>& other)
        : _success(other._success)
    {
        if (other._success)
            storage.val = other.storage.val;
    };
    result(result<T>&& other)
        : _success(other._success)
    {
        if (other._success)
            storage.val = other.storage.val;
    };

    template <class... Args>
    result(Args&&... args)
        : storage((args)...)
        , _success(true)
    {
    }

    ~result()
    {
        if (_success)
            storage.erase();
    }

    constexpr explicit operator bool() const
    {
        return _success;
    };


    constexpr const std::string& error() const
    {
        return _error;
    }
    constexpr bool failed() const
    {
        return not _success;
    };
    constexpr bool ok() const
    {
        return _success;
    };

    constexpr auto operator-> () const
    {
        return storage.valptr();
    };
    constexpr auto operator-> ()
    {
        return storage.valptr();
    };
    constexpr auto operator*() const&
    {
        return storage.valref();
    };
    constexpr auto operator*() &
    {
        return storage.valref();
    };
    constexpr const auto&& operator*() const&&
    {
        return storage.valref();
    };
    constexpr auto&& operator*() &&
    {
        return storage.valref();
    };


    constexpr auto value()
    {
        if (not _success)
            throw std::range_error("bad result: " + _error);
        return storage.valref();
    }
    constexpr auto value_or(T&& other)
    {
        if (not _success)
            return other;
        return storage.valref();
    }
};

} /* kr */
