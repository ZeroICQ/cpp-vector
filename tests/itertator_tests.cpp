#include "catch.hpp"
#include <iterator>
#include <type_traits>
#include "vector.h"

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

        auto itA = test_vector.begin();
        auto itB = itA;

        itB += 30;

        REQUIRE(itA != itB);
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

    SECTION("can be incremented")
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

    SECTION("can be decremented")
    {
        auto it = test_vector.begin();
        for (int i = 0; i < 10; i++) {
            it++;
        }

        REQUIRE(*it-- == 10);
        REQUIRE(*--it == 8);
    }

    SECTION("comparision")
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

    SECTION("Iterator - n")
    {
        auto it = test_vector.begin();
        it += 50;
        auto itA = it - 12;

        REQUIRE(*it == 50);
        REQUIRE(*itA == 38);
    }

    SECTION("Iterator - Iterator")
    {
        auto itA = test_vector.begin();
        auto itB = itA;

        itA += 2;
        itB += 50;

        REQUIRE(itB - itA == 48);
        REQUIRE(itA - itB == -48);
    }

    SECTION("Iterator += n")
    {
        auto it = test_vector.begin();

        it += 10;

        REQUIRE(*it == 10);

        it += 5;
        REQUIRE(*it == 15);
    }

    SECTION("Iterator -= n")
    {
        auto it = test_vector.begin();

        it = it + 20;

        it -= 10;

        REQUIRE(*it == 10);
        it -= 1;
        REQUIRE(*it == 9);
    }

    SECTION("[]")
    {
        auto itA = test_vector.begin();
        REQUIRE(itA[40] == 40);
        REQUIRE(itA[11] == 11);
    }

    SECTION("relational operators")
    {
        auto itA = test_vector.begin();
        auto itB = test_vector.begin();

        itA += 50;

        REQUIRE(itB < itA);
        REQUIRE(itB <= itA);

        REQUIRE_FALSE(itB > itA);
        REQUIRE_FALSE(itB >= itA);

        REQUIRE(itA > itB);
        REQUIRE(itA >= itA);

    }

    SECTION("std::distance")
    {
        auto itA = test_vector.begin();
        auto itB = test_vector.begin();

        REQUIRE(itA == itB);
        REQUIRE_FALSE(itA == ++itB);
        REQUIRE(itA != itB);
        REQUIRE(++itA == itB);
    }

    SECTION("std::advance")
    {
        auto it = test_vector.begin();
        std::advance(it, 5);
        REQUIRE(*it == 5);
    }

    SECTION("std::next, std::prev")
    {
        auto it = test_vector.begin();
        it += 40;

        auto itA = std::next(it);
        auto itB = std::prev(it);

        REQUIRE(*itA == 41);
        REQUIRE(*itB == 39);
    }
}

TEST_CASE("Iterator creation")
{
    atl::vector<int> test_vector(100, 228);
    SECTION(".begin()")
    {
//        auto kek = test_vector.begin();
//        int a = 10;
    }
}