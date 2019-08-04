#pragma once

#include <string>
#include <stdexcept>
#include <utility>

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

        ~storage_t() {};
    };

} /* _result_detail */

enum whatever
{
    FAILED = 1
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
        : _success(false) {(void)(f);};
    result(whatever f, const std::string& error)
        : _success(false), _error(error) {(void)(f);};
    result(whatever f, const char* error)
        : _success(false), _error(error) {(void)(f);};

    result()
        : _success(false) {};

    result(const T& other)
        : storage(other)
        , _success(true) {};

    result(T&& other)
        : storage(other)
        , _success(true) {};

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
            storage.val.T::~T();
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
    constexpr bool success() const
    {
        return _success;
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
        if (not _success)
            throw std::range_error("bad result: "+_error);
        return storage.val;
    }
    constexpr T& value_or(T&& other)
    {
        if (not _success)
            return other;
        return storage.val;
    }

};

} /* kr */
