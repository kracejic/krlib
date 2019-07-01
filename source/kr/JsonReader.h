#pragma once

#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>


namespace kr
{

namespace Json
{
    namespace _detail
    {
        enum class Type : int8_t
        {
            object,
            array,
            string,
            number,
            true_,
            false_,
            null,
            keyval
        };

        class TreeNode
        {
          public:
            TreeNode(Type _type, char* start)
                : type(_type)
                , text_start(start) {};
            TreeNode(Type _type, char* start, char* end)
                : type(_type)
                , text_start(start)
                , text_end(end) {};
            Type type {Type::null};
            uint32_t size {1};
            // indexes ins
            char* text_start {0};
            char* text_end {0};
        };
    }

    class Value
    {
      protected:
        _detail::TreeNode* me {nullptr};

      public:
        Value() {};
        Value(_detail::TreeNode* _it)
            : me(_it) {};
        Value& operator++()
        {
            me = me + me->size;
            return *this;
        }
        Value& operator*()
        {
            return *this;
        }
        bool operator!=(const Value& rhs) const
        {
            return me != rhs.me;
        }

        bool operator!=(std::nullptr_t p) const
        {
            (void)p;
            return not isNull();
        }
        bool operator==(std::nullptr_t p) const
        {
            (void)p;
            return isNull();
        }

        template <class T>
        T _or(T def_value) const
        {
            if (me == nullptr)
                return def_value;
            return (T) * this;
        }
        std::string _or(const char* def_value) const
        {
            if (me == nullptr)
                return def_value;
            return str();
        }

        template <class T>
        T get_or(const char* key, T def_value) const
        {
            if (me == nullptr)
                return def_value;
            for (auto& keyval : *this)
                if (keyval.key() == key)
                    return (T)keyval.value();
            return def_value;
        }
        std::string get_or(const char* key, const char* def_value) const
        {
            if (me == nullptr)
                return def_value;
            for (auto& keyval : *this)
                if (keyval.key() == key)
                    return keyval.value().str();
            return def_value;
        }

        /// Object accessor.
        Value operator[](const std::string& key) const
        {
            for (auto& keyval : *this)
                if (keyval.key() == key)
                    return keyval.value();
            return {};
        }

        /// Object accessor.
        Value operator[](const char* key) const
        {
            for (auto& keyval : *this)
                if (keyval.key() == key)
                    return keyval.value();
            return {};
        }

        /// Array accessor.
        Value operator[](int index) const
        {
            Value ret = this->begin();
            Value end = this->end();
            for (int i = 0; i < index && ret != end; ++i)
                ++ret;
            return ret;
        }

        /// Returns true when object does exists.
        bool exists() const
        {
            return me != nullptr;
        }

        /// Number of elements in array or in object.
        size_t size() const
        {
            throwIfInvalid();
            Value ret = this->begin();
            size_t count = 0;
            while (ret != this->end())
            {
                ++count;
                ++ret;
            }
            return count;
        }

        /// Returns key when iterating over key/value pair.
        std::string key() const
        {
            throwIfInvalid();
            Value key = me + 1;
            return key.str();
        }

        /// Returns value when iterating over key/value pair.
        Value value() const
        {
            throwIfInvalid();
            Value val = me + 2;
            return val;
        }

        Value begin() const
        {
            if (me == nullptr)
                return {nullptr};
            return {me + 1};
        }
        Value end() const
        {
            if (me == nullptr)
                return {nullptr};
            return {me + me->size};
        }
        std::string raw() const
        {
            throwIfInvalid();
            return {me->text_start, me->text_end};
        }
        std::string str() const
        {
            throwIfInvalid();
            std::string ret = "";
            for (char* ch = me->text_start; ch < me->text_end; ++ch)
            {
                if (*ch == '\\')
                {
                    ++ch;
                    switch (*ch)
                    {
                        case '\\':
                            ret.push_back('\\');
                            break;
                        case 'n':
                            ret.push_back('\n');
                            break;
                        case 't':
                            ret.push_back('\t');
                            break;
                        case 'b':
                            ret.push_back('\b');
                            break;
                        case 'f':
                            ret.push_back('\f');
                            break;
                        case 'r':
                            ret.push_back('\r');
                            break;
                        case '"':
                            ret.push_back('"');
                            break;
                        default:
                            ret.push_back(*ch);
                    }
                }
                else
                    ret.push_back(*ch);
            }
            return ret;
        }

        void throwIfInvalid() const
        {
            if (me == nullptr)
                throw std::runtime_error {"Element is not in here."};
        }

        // clang-format off
        std::string asString() const { throwIfInvalid(); return str(); }
        bool asBool() const { throwIfInvalid(); return me->type == _detail::Type::true_; }
        int asInt() const { throwIfInvalid(); return strtol(me->text_start, nullptr, 10); }
        long asLong() const { throwIfInvalid(); return strtol(me->text_start, nullptr, 10); }
        float asFloat() const { throwIfInvalid(); return strtof(me->text_start, nullptr); }
        double asDouble() const { throwIfInvalid(); return strtod(me->text_start, nullptr); }

        operator std::string() const { throwIfInvalid(); return str(); }
        operator bool() const { throwIfInvalid(); return me->type == _detail::Type::true_; }
        operator int() const { throwIfInvalid(); return strtol(me->text_start, nullptr, 10); }
        operator long() const { throwIfInvalid(); return strtol(me->text_start, nullptr, 10); }
        operator float() const { throwIfInvalid(); return strtof(me->text_start, nullptr); }
        operator double() const { throwIfInvalid(); return strtod(me->text_start, nullptr); }

        bool operator==(const std::string& rhs) const { return operator std::string() == rhs; }
        bool operator==(const char* rhs) const { return operator std::string() == rhs; }
        bool operator==(long rhs) const { return operator long() == rhs; }
        bool operator==(int rhs) const { return operator int() == rhs; }
        bool operator==(float rhs) const { return operator float() == rhs; }
        bool operator==(double rhs) const { return operator double() == rhs; }
        bool operator==(bool rhs) const { return operator bool() == rhs; }

        bool operator!=(const std::string& rhs) const { return operator std::string() != rhs; }
        bool operator!=(const char* rhs) const { return operator std::string() != rhs; }
        bool operator!=(long rhs) const { return operator long() != rhs; }
        bool operator!=(int rhs) const { return operator int() != rhs; }
        bool operator!=(float rhs) const { return operator float() != rhs; }
        bool operator!=(double rhs) const { return operator double() != rhs; }
        bool operator!=(bool rhs) const { return operator bool() != rhs; }

        bool isNull() const { throwIfInvalid(); return me->type == _detail::Type::null; }
        bool isNumber() const { throwIfInvalid(); return me->type == _detail::Type::number; }
        bool isString() const { throwIfInvalid(); return me->type == _detail::Type::string; }
        bool isObject() const { throwIfInvalid(); return me->type == _detail::Type::object; }
        bool isArray() const { throwIfInvalid(); return me->type == _detail::Type::array; }
        // clang-format on
    };

    /**
     * Has ~2x memory overhead (plus string), but is fast.
     */
    class Reader : public Value
    {
      private:
      public:
        std::string text;
        std::vector<_detail::TreeNode> tree;

        void _parse()
        {
            std::vector<size_t> st;

            for (size_t i = 0; i < text.size(); ++i)
            {
                char& ch = text[i];

                switch (ch)
                {
                    case '"':
                        if (!st.empty() &&
                            tree[st.back()].type == _detail::Type::object)
                        {
                            st.emplace_back(tree.size());
                            tree.emplace_back(_detail::Type::keyval, &text[i]);
                        }
                        tree.emplace_back(_detail::Type::string, &text[i + 1]);
                        do
                        {
                            if (text[i] == '\\')
                                ++i;
                            ++i;
                        } while (text[i] != '"');
                        tree.back().text_end = &text[i];
                        break;

                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '-':
                    case '+':
                    case '.':
                        tree.emplace_back(_detail::Type::number, &text[i]);
                        do
                        {
                            ++i;
                        } while (std::isdigit(text[i]) || text[i] == '.');
                        tree.back().text_end = &text[i];
                        --i;
                        break;

                    case 'f':
                    case 'F':
                        if (text[i + 1] == 'a' && text[i + 2] == 'l' &&
                            text[i + 3] == 's' && text[i + 4] == 'e')
                        {
                            tree.emplace_back(
                                _detail::Type::false_, &text[i], &text[i + 5]);
                            i += 4;
                        }
                        break;
                    case 't':
                    case 'T':
                        if (text[i + 1] == 'r' && text[i + 2] == 'u' &&
                            text[i + 3] == 'e')
                        {
                            tree.emplace_back(
                                _detail::Type::true_, &text[i], &text[i + 4]);
                            i += 3;
                        }
                        break;
                    case 'n':
                        if (text[i + 1] == 'u' && text[i + 2] == 'l' &&
                            text[i + 3] == 'l')
                        {
                            tree.emplace_back(
                                _detail::Type::null, &text[i], &text[i + 4]);
                            i += 3;
                        }
                        break;

                    case '[':
                        st.emplace_back(tree.size());
                        tree.emplace_back(_detail::Type::array, &text[i]);
                        break;
                    case ']':
                        if (st.empty())
                            throw std::runtime_error {
                                "invalid json array closing"};
                        if (tree[st.back()].type != _detail::Type::array)
                            throw std::runtime_error {
                                "invalid json array closing"};
                        tree[st.back()].text_end = &text[i + 1];
                        tree[st.back()].size = tree.size() - st.back();
                        st.pop_back();
                        break;
                    case '{':
                        st.emplace_back(tree.size());
                        tree.emplace_back(_detail::Type::object, &text[i]);
                        break;
                    case '}':
                        if (st.empty())
                            throw std::runtime_error {
                                "invalid json object closing"};

                        if (tree[st.back()].type == _detail::Type::keyval)
                        {
                            tree[st.back()].text_end = &text[i];
                            tree[st.back()].size = tree.size() - st.back();
                            st.pop_back();
                        }
                        if (tree[st.back()].type != _detail::Type::object)
                            throw std::runtime_error {
                                "invalid json object closing"};
                        tree[st.back()].text_end = &text[i + 1];
                        tree[st.back()].size = tree.size() - st.back();
                        st.pop_back();
                        break;
                    case ',':
                        if (!st.empty() &&
                            tree[st.back()].type == _detail::Type::keyval)
                        {
                            tree[st.back()].text_end = &text[i];
                            tree[st.back()].size = tree.size() - st.back();
                            st.pop_back();
                        }
                        break;
                    default:
                        break;
                }
            }
            me = &(tree.front());
        }

      public:
        Reader() {};

        Reader(std::string&& input)
            : text(input)
        {
            _parse();
        }

        Reader(const char* input)
            : text(input)
        {
            _parse();
        }

        void parse(std::string&& input)
        {
            tree.clear();
            text = input;
            _parse();
        }
    };
}
}
