#include "catch.hpp"
#include "kr/string.h"

TEST_CASE("krstring")
{
    REQUIRE(kr::itostr(1) == "1");
    REQUIRE(kr::itostr(5) == "5");
    REQUIRE(kr::itostr(165) == "165");
    REQUIRE(kr::itostr(234023) == "234023");
    REQUIRE(kr::itostr(0) == "0");
    REQUIRE(kr::itostr(-200) == "-200");
    REQUIRE(kr::itostr(-1) == "-1");
    REQUIRE(kr::itostr(-124) == "-124");
    REQUIRE(kr::itostr(266) == "266");
    REQUIRE(kr::itostr(255) == "255");
}

TEST_CASE("Human readable")
{
    REQUIRE(kr::human_readable(1024) == "1024");
    REQUIRE(kr::human_readable(102400) == "100k");
    REQUIRE(kr::human_readable(10240) == "10k");
    REQUIRE(kr::human_readable(1024 * 5) == "5120");
    REQUIRE(kr::human_readable(1073741824) == "1024M");
    REQUIRE(kr::human_readable(1073741824L * 50L) == "50G");
}

TEST_CASE("starts_with")
{
    std::string a = "test";

    REQUIRE(kr::starts_with(a, "t") == true);
    REQUIRE(kr::starts_with(a, "te") == true);
    REQUIRE(kr::starts_with(a, "tes") == true);
    REQUIRE(kr::starts_with(a, "test") == true);

    REQUIRE(kr::starts_with(a, "testa") == false);
    std::string b = "test long string which would be probabably on heap, "
                    ".....................................";
    REQUIRE(kr::starts_with(b,
                "test long string which would be probabably on heap, "
                "......................................") == false);
    REQUIRE(kr::starts_with(b, "testa") == false);
    REQUIRE(kr::starts_with(b, "a") == false);
    REQUIRE(kr::starts_with(b, "test") == true);
    REQUIRE(kr::starts_with(b, "t") == true);
}

TEST_CASE("ends_with")
{
    std::string a = "test";
    REQUIRE(kr::ends_with(a, "t") == true);
    REQUIRE(kr::ends_with(a, "st") == true);
    REQUIRE(kr::ends_with(a, "est") == true);
    REQUIRE(kr::ends_with(a, "test") == true);
    REQUIRE(kr::ends_with(a, "atest") == false);
    REQUIRE(kr::ends_with(a, "a") == false);
}

TEST_CASE("strip")
{
    std::string a = "  test  ";
    REQUIRE(kr::trimmed(a) == "test");
    REQUIRE(kr::trimmed(a, true, false) == "test  ");
    REQUIRE(kr::trimmed(a, false, true) == "  test");
}

TEST_CASE("split")
{
    std::string a = "point1  p2\n\t   p3 p4";
    REQUIRE(
        kr::split(a) == std::vector<std::string>({"point1", "p2", "p3", "p4"}));
    REQUIRE(kr::split(a, "\t\n ", 2) ==
        std::vector<std::string>({"point1", "p2", "p3 p4"}));
    std::string b = "   point1  p2\n\t   p3 p4";
    REQUIRE(kr::split(b, "\t\n ", 2) ==
        std::vector<std::string>({"point1", "p2", "p3 p4"}));
    REQUIRE(kr::split(b, "\t\n ", 3) ==
        std::vector<std::string>({"point1", "p2", "p3", "p4"}));
    REQUIRE(kr::split(b, "\t\n ", 1) ==
        std::vector<std::string>({"point1", "p2\n\t   p3 p4"}));
}

TEST_CASE("lower/upper case")
{
    REQUIRE(kr::toLowerCase("TesT") == "test");
    REQUIRE(kr::toUpperCase("TesT") == "TEST");

    std::string a = "TesT";
    kr::transformToLowerCase(a);
    REQUIRE(a == "test");
    kr::transformToUpperCase(a);
    REQUIRE(a == "TEST");
}
