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
        enum class Type
        {
            object,
            array,
            string,
            number,
            bolean,
            null,
            keyval
        };

        class TreeNode
        {
          public:
            TreeNode(Type _type, size_t start)
                : type(_type)
                , text_start(start){};
            Type type{Type::null};
            // indexes ins
            size_t text_start{0};
            size_t text_end{0};

            int tree_end{0};
        };

        class Json
        {
            TreeNode* me;
        };
    }

    class Array
    {
    };
    class Object
    {
    };

    class Reader
    {
      private:
        std::string text;
        std::vector<_detail::TreeNode> tree;

        void _parse()
        {
            std::vector<size_t> st;
            bool inText = false;
            bool inNumber = false;
            for (size_t i = 0; i < text.size(); ++i)
            {
                char& ch = text[i];
                if (inText && ch != '"')
                    continue;
                if (inNumber && (std::isdigit(ch) || ch == '.'))
                    continue;

                switch (ch)
                {
                    case '"':
                        if (not inText)
                            tree.emplace_back(_detail::Type::string, i + 1);
                        else
                            tree.back().text_end = i - 1;
                        inText = !inText;
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
                        inNumber = true;
                        tree.emplace_back(_detail::Type::number, i);
                        break;

                    case '[':
                        st.emplace_back(tree.size());
                        tree.emplace_back(_detail::Type::array, i);
                        break;
                    case ']':
                        tree[st.back()].text_end = i;
                        st.pop_back();
                        break;
                    case '{':
                        st.emplace_back(tree.size());
                        tree.emplace_back(_detail::Type::object, i);
                        break;
                    case '}':
                        tree[st.back()].text_end = i;
                        st.pop_back();
                        break;
                    default: break;
                }
            }
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
        bool isNull()
        {
            return tree.front().type == _detail::Type::null;
        };
        bool isBool()
        {
            return tree.front().type == _detail::Type::bolean;
        };
        bool isNumber()
        {
            return tree.front().type == _detail::Type::number;
        };
        bool isString()
        {
            return tree.front().type == _detail::Type::string;
        };
        bool isObject()
        {
            return tree.front().type == _detail::Type::object;
        };
        bool isArray()
        {
            return tree.front().type == _detail::Type::array;
        };
        // TODO more

        void parse(std::string&& input)
        {
            tree.clear();
            text = input;
        }
    };
}
}
