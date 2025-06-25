#pragma once

#include <cstring>
#include <string>
#include <cstdint>
#if __cplusplus > 201402L
#include <string_view>
#endif

namespace kr
{


template <int SIZE, class SIZE_TYPE=uint8_t>
class fixedString
{
  public:
    char data[SIZE];
    SIZE_TYPE len = 0;

    constexpr auto begin()
    {
        return (char*)data;
    }
    constexpr auto end()
    {
        return begin() + len;
    }
    constexpr auto begin() const
    {
        return (const char*)data;
    }
    constexpr auto end() const
    {
        return begin() + len;
    }
    constexpr SIZE_TYPE length() const
    {
        return len;
    }
    constexpr SIZE_TYPE size() const
    {
        return len;
    }
    constexpr SIZE_TYPE max_size() const
    {
        return SIZE;
    }

    constexpr char& operator[](SIZE_TYPE index)
    {
        return data[index];
    }
    constexpr const char& operator[](SIZE_TYPE index) const
    {
        return data[index];
    }

    void push_back(char r)
    {
        data[len] = r;
        len++;
    }
    void append(char r)
    {
        data[len] = r;
        len++;
    }
    void append(const char* r)
    {

        strncpy(&(data[len]), r, SIZE - len);
        // len = SIZE < strlen(r) ? SIZE : text.size();
    }
    void append(const std::string& text)
    {
        strncpy(&(data[len]), text.c_str(), SIZE - len);
        len = SIZE < text.size() ? SIZE : text.size();
    }

    fixedString()
    {
    };
    fixedString(char ch)
    {
        for (SIZE_TYPE i = 0; i < SIZE; ++i)
            data[i] = ch;
    };
    fixedString(const char* txt)
    {
        strncpy(data, txt, SIZE);
        len = strlen(data);
    };
    fixedString(const std::string& text)
    {
        strncpy(data, text.c_str(), SIZE);
        len = SIZE < text.size() ? SIZE : text.size();
    };
#if __cplusplus > 201402L
    fixedString(const std::string_view text)
    {
        len = SIZE < text.size() ? SIZE : text.size();
        strncpy(data, text.data(), len);
        data[SIZE - 1] = '\0';
    };

    operator std::string_view()
    {
        return {data, len};
    }
    std::string_view asString_view()
    {
        return {data, len};
    }
#endif
    std::string asString() const
    {
        return {data, len};
    }


    constexpr bool operator==(const fixedString<SIZE>& r) const
    {
        if (r.size() != len)
            return false;
        for (SIZE_TYPE i = 0; i < len; ++i)
            if (data[i] != r.data[i])
                return false;
        return true;
    }
    constexpr bool operator!=(const fixedString<SIZE>& r) const
    {
        return not this->operator==(r);
    }
    constexpr bool operator==(const char* r) const
    {
        SIZE_TYPE rsize = strlen(r);
        if (rsize != len)
            return false;
        for (SIZE_TYPE i = 0; i < len; ++i)
            if (data[i] != r[i])
                return false;
        return true;
    }
    constexpr bool operator!=(const char* r) const
    {
        return not this->operator==(r);
    }
    constexpr bool operator==(const std::string& r) const
    {
        if (r.size() != len)
            return false;
        for (SIZE_TYPE i = 0; i < len; ++i)
            if (data[i] != r[i])
                return false;
        return true;
    }
    constexpr bool operator!=(const std::string& r) const
    {
        return not this->operator==(r);
    }
#if __cplusplus > 201402L
    constexpr bool operator==(const std::string_view r) const
    {
        if (r.size() != len)
            return false;
        for (SIZE_TYPE i = 0; i < len; ++i)
            if (data[i] != r[i])
                return false;
        return true;
    }
    constexpr bool operator!=(const std::string_view r) const
    {
        return not this->operator==(r);
    }
#endif

  private:
};

} /* kr */
