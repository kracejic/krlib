#include <string>

namespace kr
{

class JsonWritterTo
{
  protected:
    bool firstElement = true;

  private:
    std::string& target;

    void comma()
    {
        if (not firstElement)
            target.push_back(',');
    }

  public:
    JsonWritterTo() = delete;
    JsonWritterTo(std::string& target)
        : target(target){};
    JsonWritterTo(std::string& target, size_t size)
        : target(target)
    {
        target.reserve(size);
    };

    void clear()
    {
        target = "";
        firstElement = true;
    }

    /// Reserves size in memory
    void reserve(size_t size)
    {
        target.reserve(size);
    };


    void startObject()
    {
        comma();
        target.append("{");
        firstElement = true;
    };
    void startObject(const std::string& name)
    {
        comma();
        target.push_back('"');
        target.append(name);
        target.append("\":{");
        firstElement = true;
    }
    void endObject()
    {
        target.append("}");
        firstElement = false;
    };

    void startArray()
    {
        comma();
        target.append("[");
        firstElement = true;
    };
    void startArray(const std::string& name)
    {
        comma();
        target.push_back('"');
        target.append(name);
        target.append("\":[");
        firstElement = true;
    };
    void endArray()
    {
        target.append("]");
        firstElement = false;
    };

    template <class T>
    void put(const std::string& name, const T value)
    {
        comma();
        target.push_back('"');
        target.append(name);
        target.append("\":");
        target.append(std::to_string(value));
        firstElement = false;
    };
    template <class T>
    void put(const T value)
    {
        comma();
        target.append(std::to_string(value));
        firstElement = false;
    };

    // const char* specialization
    void put(const std::string& name, const char* value)
    {
        comma();
        target.push_back('"');
        target.append(name);
        target.append("\":\"");
        target.append(value);
        target.push_back('"');
        firstElement = false;
    };
    void put(const char* value)
    {
        comma();
        target.push_back('"');
        target.append(value);
        target.push_back('"');
        firstElement = false;
    };

    // string specialization
    void put(const std::string& name, const std::string& value)
    {
        comma();
        target.push_back('"');
        target.append(name);
        target.append("\":\"");
        target.append(value);
        target.push_back('"');
        firstElement = false;
    };
    void put(const std::string& value)
    {
        comma();
        target.push_back('"');
        target.append(value);
        target.push_back('"');
        firstElement = false;
    };

    // bool specialization
    void put(const std::string& name, bool value)
    {
        comma();
        target.push_back('"');
        target.append(name);
        if (value)
            target.append(R"(":true)");
        else
            target.append(R"(":false)");
        firstElement = false;
    };
    void put(bool value)
    {
        comma();
        if (value)
            target.append("true");
        else
            target.append("false");
        firstElement = false;
    };

    void putRaw(const std::string& name, const std::string value)
    {
        comma();
        target.push_back('"');
        target.append(name);
        target.append("\":");
        target.append(value);
        firstElement = false;
    };

    const std::string& get() const
    {
        return target;
    };
};


class JsonWritter : public JsonWritterTo
{
  private:
    std::string text = "";

  public:
    JsonWritter()
        : JsonWritterTo(text){};
    JsonWritter(std::string& target) = delete;
    JsonWritter(size_t size)
        : JsonWritterTo(text, size){};

    /// Hands over ownership of created JSON string
    std::string&& moveText()
    {
        firstElement = true;
        return std::move(std::move(text));
    };

    static inline std::string escape(const std::string& in)
    {
        std::string target = "";
        target.reserve(in.size());
        for (auto ch = in.cbegin(); ch != in.cend(); ++ch)
        {
            switch (*ch)
            {
                case '"': target.append("\\\""); break;
                case '\b': target.append("\\b"); break;
                case '\n': target.append("\\n"); break;
                case '\f': target.append("\\f"); break;
                case '\r': target.append("\\r"); break;
                case '\t': target.append("\\t"); break;
                case '\\': target.append("\\\\"); break;
                default: target.push_back(*ch); break;
            }
        }
        return target;
    }
};
}
