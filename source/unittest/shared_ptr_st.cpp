#ifdef UNIT_TESTS
#include "kr/shared_ptr_st.h"
#include "kr/canaryobject.h"

#include "catch.hpp"

TEST_CASE("shared_ptr_st")
{

    kr::CanaryObject::initStates();

    {
        kr::shared_ptr_st<kr::CanaryObject> ptr;
        REQUIRE(ptr.use_count() == 0);

        ptr = new kr::CanaryObject(2, 1);
        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
        REQUIRE(ptr.use_count() == 1);
        REQUIRE((*ptr).id == 1);
        REQUIRE((*ptr).val == 2);
        REQUIRE(ptr->id == 1);
        REQUIRE(ptr->val == 2);
    }
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}

TEST_CASE("shared_ptr_st reset")
{

    kr::CanaryObject::initStates();

    {
        kr::shared_ptr_st<kr::CanaryObject> ptr;
        REQUIRE(ptr.use_count() == 0);

        ptr = new kr::CanaryObject(2, 1);
        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
        REQUIRE(ptr.use_count() == 1);

        REQUIRE(ptr);
        ptr.reset();
        REQUIRE(ptr.use_count() == 0);
        REQUIRE(not ptr);

        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
    }
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}

TEST_CASE("shared_ptr_st copy assigment")
{

    kr::CanaryObject::initStates();

    {
        kr::shared_ptr_st<kr::CanaryObject> ptr2;
        {
            kr::shared_ptr_st<kr::CanaryObject> ptr;
            REQUIRE(ptr.use_count() == 0);

            ptr = new kr::CanaryObject(1, 1);
            REQUIRE(kr::CanaryObject::states[1] ==
                kr::CanaryObject::State::construct2);
            REQUIRE(ptr.use_count() == 1);

            ptr2 = ptr;
            REQUIRE(ptr.use_count() == 2);
            REQUIRE(ptr2.use_count() == 2);
            REQUIRE(not ptr.unique());
            REQUIRE(not ptr2.unique());
        }
        REQUIRE(ptr2.use_count() == 1);
        REQUIRE(ptr2.unique());
        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
    }
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}


TEST_CASE("shared_ptr_st move assigment")
{

    kr::CanaryObject::initStates();

    {
        kr::shared_ptr_st<kr::CanaryObject> ptr2;
        {
            kr::shared_ptr_st<kr::CanaryObject> ptr;
            REQUIRE(ptr.use_count() == 0);

            ptr = new kr::CanaryObject(1, 1);
            REQUIRE(kr::CanaryObject::states[1] ==
                kr::CanaryObject::State::construct2);
            REQUIRE(ptr.use_count() == 1);

            ptr2 = std::move(ptr);
            REQUIRE(ptr.use_count() == 0);
            REQUIRE(ptr2.use_count() == 1);
        }
        REQUIRE(ptr2.use_count() == 1);
        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
    }
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}

TEST_CASE("shared_ptr_st move construct")
{

    kr::CanaryObject::initStates();

    {
        kr::shared_ptr_st<kr::CanaryObject> ptr;
        REQUIRE(ptr.use_count() == 0);

        ptr = new kr::CanaryObject(1, 1);
        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
        REQUIRE(ptr.use_count() == 1);

        kr::shared_ptr_st<kr::CanaryObject> ptr2(std::move(ptr));
        REQUIRE(ptr.use_count() == 0);
        REQUIRE(ptr2.use_count() == 1);
        REQUIRE(ptr2.unique());
        REQUIRE(not ptr.unique());
    }
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}

TEST_CASE("shared_ptr_st constructor")
{

    kr::CanaryObject::initStates();

    {
        auto ptr =
            kr::shared_ptr_st<kr::CanaryObject>(new kr::CanaryObject(1, 1));
        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
        REQUIRE(ptr.use_count() == 1);
    }
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}

TEST_CASE("make_shared_st")
{

    kr::CanaryObject::initStates();

    {
        auto ptr = kr::make_shared_st<kr::CanaryObject>(1, 1);
        REQUIRE(
            kr::CanaryObject::states[1] == kr::CanaryObject::State::construct2);
        REQUIRE(ptr.use_count() == 1);
    }
    REQUIRE(kr::CanaryObject::states[1] == kr::CanaryObject::State::destruct);
}

TEST_CASE("shared_ptr_st swap")
{
    auto ptr1 = kr::make_shared_st<kr::CanaryObject>(1, 1);
    auto ptr2 = kr::make_shared_st<kr::CanaryObject>(2, 2);

    REQUIRE(ptr1.unique());
    REQUIRE(ptr1->val == 1);
    REQUIRE(ptr2.unique());
    REQUIRE(ptr2->val == 2);

    std::swap(ptr1, ptr2);

    REQUIRE(ptr1.unique());
    REQUIRE(ptr1->val == 2);
    REQUIRE(ptr1);
    REQUIRE(ptr2.unique());
    REQUIRE(ptr2->val == 1);
    REQUIRE(ptr2);
}
#endif
