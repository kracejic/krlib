#include "catch.hpp"
#include "kr/util/base64.h"

#include <string>

using namespace std;

TEST_CASE("Base64")
{

    std::vector<unsigned char> vec1 {1,5,8,9,7,36};
    auto x = kr::Base64::encode(&vec1[0], vec1.size());

    auto ret = kr::Base64::decode(x);

    REQUIRE(vec1 == ret);
}
