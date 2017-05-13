#include <string>

namespace kr
{

template <typename T, typename Tag>
struct NamedType
{
    explicit NamedType(T const& value)
        : v(value){};
    explicit NamedType(T&& value)
        : v(value){};
    T v;


    bool operator!=(const NamedType& rhs) const
    {
        return this->v != rhs.v;
    }
    bool operator==(const NamedType& rhs) const
    {
        return this->v == rhs.v;
    }
};

template <typename T, typename Tag>
std::string to_string(const kr::NamedType<T, Tag>& val)
{
    return std::to_string(val.v);
}

//-----------------------------------------------------------------------------
// NamedNumberType

template <typename T, typename Tag>
struct NamedNumberType
{
    explicit NamedNumberType(T const& value)
        : v(value){};
    explicit NamedNumberType(T&& value)
        : v(value){};
    T v;


    bool operator!=(const NamedNumberType& rhs) const
    {
        return this->v != rhs.v;
    }
    bool operator==(const NamedNumberType& rhs) const
    {
        return this->v == rhs.v;
    }


    NamedNumberType& operator+=(const NamedNumberType& other)
    {
        this->v += other.v;
        return *this;
    }
    NamedNumberType& operator-=(const NamedNumberType& other)
    {
        this->v -= other.v;
        return *this;
    }
    NamedNumberType& operator/=(const NamedNumberType& other)
    {
        this->v /= other.v;
        return *this;
    }
    NamedNumberType& operator*=(const NamedNumberType& other)
    {
        this->v *= other.v;
        return *this;
    }

    bool operator>(const NamedNumberType& other)
    {
        return this->v > other.v;
    }
    bool operator>=(const NamedNumberType& other)
    {
        return this->v >= other.v;
    }
    bool operator<(const NamedNumberType& other)
    {
        return this->v < other.v;
    }
    bool operator<=(const NamedNumberType& other)
    {
        return this->v <= other.v;
    }
};

template <typename T, typename Tag>
NamedNumberType<T,Tag> operator+(NamedNumberType<T,Tag> a, const NamedNumberType<T,Tag>& b)
{
    a += b;
    return a;
}
template <typename T, typename Tag>
NamedNumberType<T,Tag> operator-(NamedNumberType<T,Tag> a, const NamedNumberType<T,Tag>& b)
{
    a -= b;
    return a;
}
template <typename T, typename Tag>
NamedNumberType<T,Tag> operator/(NamedNumberType<T,Tag> a, const NamedNumberType<T,Tag>& b)
{
    a /= b;
    return a;
}
template <typename T, typename Tag>
NamedNumberType<T,Tag> operator*(NamedNumberType<T,Tag> a, const NamedNumberType<T,Tag>& b)
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
