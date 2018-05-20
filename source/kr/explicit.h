#include <string>

namespace kr
{

/**
 * Usage:
 *
 * ```
 * using Test1 = kr::NamedType<float, struct DistanceTag>;
 * ```
 *
 * provides ==, != and to_string functions
 */
template <typename T, typename Tag>
struct NamedType
{
    explicit NamedType(T const& value)
        : v(value){};
    explicit NamedType(T&& value)
        : v(value){};

    template <typename T_ = T>
    explicit NamedType(T&& value,
        typename std::enable_if<!std::is_reference<T_>{},
            std::nullptr_t>::type = nullptr)
        : v(std::move(value))
    {
    }

    T v;


    constexpr bool operator!=(const NamedType& rhs) const
    {
        return this->v != rhs.v;
    }
    constexpr bool operator==(const NamedType& rhs) const
    {
        return this->v == rhs.v;
    }
};

template <typename T, typename Tag>
std::string to_string(const kr::NamedType<T, Tag>& val)
{
    return std::to_string(val.v);
}

template <typename Tag>
std::string to_string(const kr::NamedType<std::string&, Tag>& val)
{
    return val.v;
}

template <typename Tag>
std::string to_string(const kr::NamedType<std::string, Tag>& val)
{
    return val.v;
}

//-----------------------------------------------------------------------------
// NamedNumberType

template <typename T, typename Tag>
struct NamedNumberType
{
    constexpr explicit NamedNumberType(T const& value)
        : v(value){};
    constexpr explicit NamedNumberType(T&& value)
        : v(value){};
    T v;


    constexpr bool operator!=(const NamedNumberType& rhs) const
    {
        return this->v != rhs.v;
    }
    constexpr bool operator==(const NamedNumberType& rhs) const
    {
        return this->v == rhs.v;
    }


    constexpr NamedNumberType& operator+=(const NamedNumberType& other)
    {
        this->v += other.v;
        return *this;
    }
    constexpr NamedNumberType& operator-=(const NamedNumberType& other)
    {
        this->v -= other.v;
        return *this;
    }
    constexpr NamedNumberType& operator/=(const NamedNumberType& other)
    {
        this->v /= other.v;
        return *this;
    }
    constexpr NamedNumberType& operator*=(const NamedNumberType& other)
    {
        this->v *= other.v;
        return *this;
    }

    constexpr bool operator>(const NamedNumberType& other) const
    {
        return this->v > other.v;
    }
    constexpr bool operator>=(const NamedNumberType& other) const
    {
        return this->v >= other.v;
    }
    constexpr bool operator<(const NamedNumberType& other) const
    {
        return this->v < other.v;
    }
    constexpr bool operator<=(const NamedNumberType& other) const
    {
        return this->v <= other.v;
    }
};

template <typename T, typename Tag>
constexpr NamedNumberType<T, Tag> operator+(
    NamedNumberType<T, Tag> a, const NamedNumberType<T, Tag>& b)
{
    a += b;
    return a;
}
template <typename T, typename Tag>
constexpr NamedNumberType<T, Tag> operator-(
    NamedNumberType<T, Tag> a, const NamedNumberType<T, Tag>& b)
{
    a -= b;
    return a;
}
template <typename T, typename Tag>
constexpr NamedNumberType<T, Tag> operator/(
    NamedNumberType<T, Tag> a, const NamedNumberType<T, Tag>& b)
{
    a /= b;
    return a;
}
template <typename T, typename Tag>
constexpr NamedNumberType<T, Tag> operator*(
    NamedNumberType<T, Tag> a, const NamedNumberType<T, Tag>& b)
{
    a *= b;
    return a;
}

template <typename T, typename Tag>
std::string to_string(const kr::NamedNumberType<T, Tag>& val)
{
    return std::to_string(val.v);
}
}
