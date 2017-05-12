#include <string>

namespace kr
{

class JsonWritterTo
{
  private:
    std::string& target;
    bool firstElement = true;

    void comma()
    {
        if (not firstElement)
            target.push_back(',');
    }

  public:
    JsonWritterTo() = delete;
    JsonWritterTo(std::string& target)
        : target(target){};

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

// const char* specialization
template <>
inline void JsonWritterTo::put<const char*>(
    const std::string& name, const char* value)
{
    comma();
    target.push_back('"');
    target.append(name);
    target.append("\":\"");
    target.append(value);
    target.push_back('"');
    firstElement = false;
};
template <>
void JsonWritterTo::put<const char*>(const char* value)
{
    comma();
    target.push_back('"');
    target.append(value);
    target.push_back('"');
    firstElement = false;
};

// string specialization
template <>
inline void JsonWritterTo::put<const std::string&>(
    const std::string& name, const std::string& value)
{
    comma();
    target.push_back('"');
    target.append(name);
    target.append("\":\"");
    target.append(value);
    target.push_back('"');
    firstElement = false;
};
template <>
void JsonWritterTo::put<const std::string&>(const std::string& value)
{
    comma();
    target.push_back('"');
    target.append(value);
    target.push_back('"');
    firstElement = false;
};

// bool specialization
template <>
inline void JsonWritterTo::put<bool>(
    const std::string& name, bool value)
{
    comma();
    target.push_back('"');
    target.append(name);
    if(value)
        target.append( R"(":true)");
    else
        target.append( R"(":false)");
    firstElement = false;
};
template <>
void JsonWritterTo::put<bool>(bool value)
{
    comma();
    if(value)
        target.append( "true");
    else
        target.append( "false");
    firstElement = false;
};

class JsonWritter : public JsonWritterTo
{
  private:
    std::string text = "";

  public:
    JsonWritter()
        : JsonWritterTo(text){};
    JsonWritter(std::string& target) = delete;

    /// Reserves size in memory
    void reserve(size_t size)
    {
        text.reserve(size);
    };

    /// Hands over ownership of created JSON string
    std::string&& moveText()
    {
        return std::move(std::move(text));
    };

    void clear() {
        text = "";
    }
};
}
