#pragma once

#include <cctype>
#include <string>
#include <vector>

#include <iostream>

namespace kr
{

namespace Json
{
    namespace _detail
    {
        enum class Type
        {
            NA,
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
            // indexes ins
            char* text_start{0};
            char* text_end{0};

            int size{1};
        };

        class Json
        {
          public:
            TreeNode* me{nullptr};
            Json(){};
            Json(TreeNode* _it)
                : me(_it){};
            Json& operator++()
            {
                me = me + me->size;
                return *this;
            }
            Json& operator*()
            {
                return *this;
            }
            bool operator!=(const Json& rhs)
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

            Json operator[](size_t index)
            {
                Json ret = this->begin();
                for (size_t i = 0; i < index; ++i)
                    ++ret;
                return ret;
            }
            size_t size()
            {
                Json ret = this->begin();
                size_t count = 0;
                while (ret != this->end())
                {
                    ++count;
                    ++ret;
                }
                return count;
            }

            Json begin()
            {
                return {me + 1};
            }
            Json end()
            {
                return {me + me->size};
            }
            std::string str()
            {
                return {me->text_start, me->text_end};
            }



            // clang-format off
            operator std::string() { return {me->text_start, me->text_end}; }
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

            bool isNull() { return me->type == _detail::Type::null; };
            bool isNumber() { return me->type == _detail::Type::number; };
            bool isString() { return me->type == _detail::Type::string; };
            bool isObject() { return me->type == _detail::Type::object; };
            bool isArray() { return me->type == _detail::Type::array; };
            // clang-format on
        };
    }

    class Array : public _detail::Json
    {
    };
    class Object : public _detail::Json
    {
    };

    class Reader : public _detail::Json
    {
      private:
      public:
        std::string text;
        std::vector<_detail::TreeNode> tree;

        void _parse()
        {
            std::vector<size_t> st;
            st.emplace_back(0);

            for (size_t i = 0; i < text.size(); ++i)
            {
                char& ch = text[i];

                switch (ch)
                {
                    case '"':
                        std::cout << "TEXT\n";
                        tree.emplace_back(_detail::Type::string, &text[i + 1]);
                        do
                        {
                            ++i;
                            std::cout << " text[i] = " << text[i] << "\n";
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
                        std::cout << "NUMBER\n";
                        tree.emplace_back(_detail::Type::number, &text[i]);
                        do
                        {
                            ++i;
                            std::cout << " text[i] = " << text[i] << "\n";
                        } while (std::isdigit(text[i]) || text[i] == '.');
                        tree.back().text_end = &text[i];
                        --i;
                        break;

                    case 'f':
                    case 'F':
                        if (text[i + 1] == 'a' && text[i + 2] == 'l' &&
                            text[i + 3] == 's' && text[i + 4] == 'e')
                        {
                            std::cout << "FALSE\n";
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
                            std::cout << "TRUE\n";
                            tree.emplace_back(
                                _detail::Type::true_, &text[i], &text[i + 4]);
                            i += 3;
                        }
                        break;
                    case 'n':
                        if (text[i + 1] == 'u' && text[i + 2] == 'l' &&
                            text[i + 3] == 'l')
                        {
                            std::cout << "NULL\n";
                            tree.emplace_back(
                                _detail::Type::null, &text[i], &text[i + 4]);
                            i += 3;
                        }
                        break;

                    case '[':
                        std::cout << "ARRAY\n";
                        st.emplace_back(tree.size());
                        tree.emplace_back(_detail::Type::array, &text[i]);
                        break;
                    case ']':
                        std::cout << "st.back() = " << st.back() << "\n";
                        tree[st.back()].text_end = &text[i + 1];
                        tree[st.back()].size = tree.size() - st.back();
                        st.pop_back();
                        break;
                    case '{':
                        std::cout << "OBJECT\n";
                        st.emplace_back(tree.size());
                        tree.emplace_back(_detail::Type::object, &text[i]);
                        break;
                    case '}':
                        tree[st.back()].text_end = &text[i + 1];
                        tree[st.back()].size = tree.size() - st.back();
                        st.pop_back();
                        break;
                    default: break;
                }
            }
            me = &(tree.front());
        }

      public:
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
        // TODO more

        void parse(std::string&& input)
        {
            tree.clear();
            text = input;
        }
    };
}
}
