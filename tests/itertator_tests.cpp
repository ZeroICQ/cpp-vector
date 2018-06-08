#include "catch.hpp"
#include "vector.h"
#include <type_traits>

TEST_CASE("Check STL requirements")
{
    using TestType = int;
    using TestVectorT = atl::vector<TestType>::iterator;

    atl::vector<int> test_vector;
    for (int i = 0; i < 100; i++) {
        test_vector.push_back(i);
    }

    SECTION("copy-assignable")
    {
        REQUIRE(std::is_copy_assignable<TestVectorT>::value);
    }

    SECTION("copy-constructible")
    {
        REQUIRE(std::is_copy_constructible<TestVectorT>::value);
    }

    SECTION("is destructible")
    {
        REQUIRE(std::is_destructible<TestVectorT>::value);
    }

    SECTION("can be derefernce")
    {
        auto it = test_vector.begin();
        it++;
        REQUIRE(*it == 1);
        ++it;
        REQUIRE(*it == 2);
    }
//    TODO: implement
//    SECTION("->")
//    {
//
//    }

    SECTION("can be incremented");
    {
        auto it = test_vector.begin();
        for (int i = 0; i < 10; i++) {
            it++;
        }

        REQUIRE(*it == 10);
        REQUIRE(*it++ == 10);
        REQUIRE(*it == 11);
        REQUIRE(*++it == 12);
    }

    SECTION("can be decremented");
    {
        auto it = test_vector.begin();
        for (int i = 0; i < 10; i++) {
            it++;
        }

        REQUIRE(*it-- == 10);
        REQUIRE(*--it == 8);
    }

    SECTION("comparision");
    {
        auto itA = test_vector.begin();
        auto itB = test_vector.begin();
        REQUIRE(itA == itB);
        REQUIRE_FALSE(itA == ++itB);
        REQUIRE(itA != itB);
        REQUIRE(++itA == itB);
    }

    SECTION("Iterator + n, n + Iterator")
    {
        auto it = test_vector.begin();

        auto itA = it + 10;

        REQUIRE(*it == 0);
        REQUIRE(*itA == 10);

        auto itB = 15 + it;

        REQUIRE(*it == 0);
        REQUIRE(*itB == 15);
    }

    SECTION("std::distance");
    {
        auto itA = test_vector.begin();
        auto itB = test_vector.begin();

        REQUIRE(itA == itB);
        REQUIRE_FALSE(itA == ++itB);
        REQUIRE(itA != itB);
        REQUIRE(++itA == itB);
    }


}

TEST_CASE("Iterator creation")
{
    atl::vector<int> test_vector(100, 228);
    SECTION(".begin()")
    {
        auto kek = test_vector.begin();
        int a = 10;
    }
}