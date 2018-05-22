# krlib

Playground/repository of simple headers.

## Components

### base64.h

Base64 helper

```
include "kr/base64.b"

std::string kr::Base64::encode(BYTE const* buf, size_t bufLen)
std::vector<BYTE> kr::Base64::decode(std::string const& encoded_string)
```

### explicit.h

Explicit type, can be numeric.

```
#include "kr/explicit.h"

using TestType1 = kr::NamedType<float, struct Test1Tag>;
using TestType2 = kr::NamedType<std::string, struct Test2Tag>;
using Distance = kr::NamedNumberType<int, struct DistanceTag>;
```

### inlineRing.h

Inline ring with interface similar to std::vector.

```
#include "kr/inlineRing.h"
kr::inlineRing8<int, 8> ring;
kr::inlineRing16<int, 512> ring; //for bigger ring, use inlineRing16 or more
```

### inlineVec.h

Inline vector with interface matching std::vector.

```
#include "kr/inlineVec.h"

kr::inlineVec<int, 10> t;
t.push_back(5);
```

### JsonReader.h

Simple json parser.

```
#include "kr/JsonReader.h"

Reader r{R"({"test":"first", "num":56, "array":[5,6,"test"]})"};

REQUIRE(r["test"] == "first");
REQUIRE(r["num"] == 56);
REQUIRE(r["array"][0] == 5);
REQUIRE(r["array"][1] == 6);
REQUIRE(r["array"][2] == "test");
```

### JsonWritter.h

Simple json stream writter.

```
#include "kr/JsonWritter.h"

kr::Json::Writter js;
js.startObject();
    js.startObject("test1");
        js.put("obj", true);
    js.endObject();
    js.startArray("array");
        js.put(1);
        js.put(false);
    js.endArray();
js.endObject();
cout<<js.get()<<endl;
```

### optional.h

C++14 compatible version of std::optional.

### shared_ptr_st.h

Non-intrusive single threaded version for shared_ptr.

### string.h

```
#include "kr/string.h"
std::string itostr(T o)
std::string human_readable(int64_t size)
bool starts_with(const std::string& text, const std::string& substring)
bool ends_with(const std::string& text, const std::string& substring)
void trim(std::string& text, bool left = true, bool right = true)
std::string trimmed(std::string text, bool left = true, bool right = true)
std::vector<std::string> split(const std::string& text,
    const std::string& delimiters = "\t\n ", int maxSplits = 0)
void transformToLowerCase(std::string& text)
void transformToUpperCase(std::string& text)
std::string toLowerCase(std::string text)
std::string toUpperCase(std::string text)
std::string lpad(std::string text, size_t target_width, char fill = ' ')
void lpadTransform(std::string& text, size_t target_width, char fill = ' ')
std::string rpad(std::string text, size_t target_width, char fill = ' ')
void rpadTransform(std::string& text, size_t target_width, char fill = ' ')
std::string format(const char* format, T num)
```

### stopwatch.h

For measuring time of code execution.

```
include "kr/stopwatch.b"

kr::Stopwatch t;
t.start();
t.lap();
cout<<t.str()<<endl;
t.lap();
double tim1 = t.s()l
t.lap();
double tim2 = t.ms()l
cout<<t.lap_str()<<endl;
KR_STOPWATCH_LAP(t);
```
### multistopwatch.h

For measuring time of code execution.

```
include "kr/stopwatch.b"

kr::MultiStopwatch t;
t.start("first operation");
t.lap("second operation");
t.lap("third operation");
t.finalize();

for (auto &op : t.results())
    cout << op << endl;
```

### logger.h

Simple logging class.

### canaryobject.h

Helper object to test constructors/destructors.

