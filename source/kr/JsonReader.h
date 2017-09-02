#pragma once

#include <cctype>
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
                , text_start(start){};
            TreeNode(Type _type, char* start, char* end)
                : type(_type)
                , text_start(start)
                , text_end(end){};
            Type type{Type::null};
            uint32_t size{1};
            // indexes ins
            char* text_start{0};
            char* text_end{0};
        };
    }

    class Value
    {
      public:
        _detail::TreeNode* me{nullptr};
        Value(){};
        Value(_detail::TreeNode* _it)
            : me(_it){};
        Value& operator++()
        {
            me = me + me->size;
            return *this;
        }
        Value& operator*()
        {
            return *this;
        }
        bool operator!=(const Value& rhs)
        {
            return me != rhs.me;
        }

        bool operator!=(std::nullptr_t p)
        {
            (void)p;
            return not isNull();
        }
        bool operator==(std::nullptr_t p)
        {
            (void)p;
            return isNull();
        }

        Value operator[](const char* key)
        {
            for (auto& keyval : *this)
                if (keyval.key() == key)
                    return keyval.value();
            // TODO throw
            return {};
        }

        template <class T>
        T _or(T def_value)
        {
            if (me == nullptr)
                return def_value;
            return (T) *this;
        }
        std::string _or(const char* def_value)
        {
            if (me == nullptr)
                return def_value;
            return str();
        }

        template <class T>
        T get_or(const char* key, T def_value)
        {
            if (me == nullptr)
                return def_value;
            for (auto& keyval : *this)
                if (keyval.key() == key)
                    return (T)keyval.value();
            return def_value;
        }
        std::string get_or(const char* key, const char* def_value)
        {
            if (me == nullptr)
                return def_value;
            for (auto& keyval : *this)
                if (keyval.key() == key)
                    return keyval.value().str();
            return def_value;
        }

        Value operator[](int index)
        {
            Value ret = this->begin();
            Value end = this->end();
            for (int i = 0; i < index && ret != end; ++i)
                ++ret;
            return ret;
        }
        bool exists()
        {
            return me != nullptr;
        }
        size_t size()
        {
            Value ret = this->begin();
            size_t count = 0;
            while (ret != this->end())
            {
                ++count;
                ++ret;
            }
            return count;
        }

        std::string key()
        {
            if (me == nullptr)
                return "";
            Value key = me + 1;
            return key.str();
        }
        Value value()
        {
            if (me == nullptr)
                return nullptr;
            Value val = me + 2;
            return val;
        }

        Value begin()
        {
            if (me == nullptr)
                return {nullptr};
            return {me + 1};
        }
        Value end()
        {
            if (me == nullptr)
                return {nullptr};
            return {me + me->size};
        }
        std::string raw()
        {
            return {me->text_start, me->text_end};
        }
        std::string str()
        {
            std::string ret = "";
            for (char* ch = me->text_start; ch < me->text_end; ++ch)
            {
                if (*ch == '\\')
                {
                    ++ch;
                    switch (*ch)
                    {
                        case '\\': ret.push_back('\\'); break;
                        case 'n': ret.push_back('\n'); break;
                        case 't': ret.push_back('\t'); break;
                        case 'b': ret.push_back('\b'); break;
                        case 'f': ret.push_back('\f'); break;
                        case 'r': ret.push_back('\r'); break;
                        case '"': ret.push_back('"'); break;
                        default:
                            ret.push_back(*ch);
                    }
                }
                else
                    ret.push_back(*ch);
            }
            return ret;
        }

        // clang-format off
        std::string asString() { return str(); }
        bool asBool() { return me->type == _detail::Type::true_; }
        int asInt() { return strtol(me->text_start, nullptr, 10); }
        long asLong() { return strtol(me->text_start, nullptr, 10); }
        float asFloat() { return strtof(me->text_start, nullptr); }
        double asDouble() { return strtod(me->text_start, nullptr); }

        operator std::string() { return str(); }
        operator bool() { return me->type == _detail::Type::true_; }
        operator int() { return strtol(me->text_start, nullptr, 10); }
        operator long() { return strtol(me->text_start, nullptr, 10); }
        operator float() { return strtof(me->text_start, nullptr); }
        operator double() { return strtod(me->text_start, nullptr); }

        bool operator==(const std::string& rhs) { return operator std::string() == rhs; }
        bool operator==(const char* rhs) { return operator std::string() == rhs; }
        bool operator==(long rhs) { return operator long() == rhs; }
        bool operator==(int rhs) { return operator int() == rhs; }
        bool operator==(float rhs) { return operator float() == rhs; }
        bool operator==(double rhs) { return operator double() == rhs; }
        bool operator==(bool rhs) { return operator bool() == rhs; }

        bool operator!=(const std::string& rhs) { return operator std::string() != rhs; }
        bool operator!=(const char* rhs) { return operator std::string() != rhs; }
        bool operator!=(long rhs) { return operator long() != rhs; }
        bool operator!=(int rhs) { return operator int() != rhs; }
        bool operator!=(float rhs) { return operator float() != rhs; }
        bool operator!=(double rhs) { return operator double() != rhs; }
        bool operator!=(bool rhs) { return operator bool() != rhs; }

        bool isNull() { return me->type == _detail::Type::null; }
        bool isNumber() { return me->type == _detail::Type::number; }
        bool isString() { return me->type == _detail::Type::string; }
        bool isObject() { return me->type == _detail::Type::object; }
        bool isArray() { return me->type == _detail::Type::array; }
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
                        tree[st.back()].text_end = &text[i + 1];
                        tree[st.back()].size = tree.size() - st.back();
                        st.pop_back();
                        break;
                    case '{':
                        st.emplace_back(tree.size());
                        tree.emplace_back(_detail::Type::object, &text[i]);
                        break;
                    case '}':
                        if (!st.empty() &&
                            tree[st.back()].type == _detail::Type::keyval)
                        {
                            tree[st.back()].text_end = &text[i];
                            tree[st.back()].size = tree.size() - st.back();
                            st.pop_back();
                        }
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
                    default: break;
                }
            }
            me = &(tree.front());
        }

      public:
        Reader(){};

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
