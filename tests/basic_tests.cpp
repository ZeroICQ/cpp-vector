#include "catch.hpp"
#include "vector.h"


class NotIntegralType
{
public:
    NotIntegralType() : large_field_a(new int[100]), large_field_b(new int[100]) {}
    NotIntegralType(NotIntegralType&& a) {
        large_field_a = a.large_field_a;
        a.large_field_a = nullptr;
        large_field_b = new int[10];
    }

    ~NotIntegralType() {
        delete[] large_field_a;
        delete[] large_field_b;
    }

private:
    int* large_field_a;
    int* large_field_b;
};

TEST_CASE("Vector create", "[create]")
{
    SECTION("Constructors")
    {
        atl::vector<double> test_vector(500);
        REQUIRE(test_vector.capacity() == 500);
        REQUIRE(test_vector.size() == 500);
        REQUIRE_FALSE(test_vector.empty());
    }

    SECTION("Empty vector")
    {
        atl::vector<bool> test_vector(0);
        REQUIRE(test_vector.capacity() == 0);
        REQUIRE(test_vector.size() == 0);
    }
}


TEST_CASE("Access", "[access]")
{
    atl::vector<int> test_vector(10);

    SECTION(".at(pos)")
    {
        int val = test_vector.at(5);
        REQUIRE(val == 0);
        REQUIRE_THROWS_AS(test_vector.at(11), std::out_of_range);
        REQUIRE_THROWS_AS(test_vector.at(-1), std::out_of_range);
    }
}

TEST_CASE("Resize", "[access]")
{
    SECTION(".reserve(50)")
    {
        atl::vector<NotIntegralType> test_vector(10);
        REQUIRE(test_vector.capacity() == 10);

        test_vector.reserve(100);
        REQUIRE(test_vector.capacity() == 100);

        test_vector.reserve(10);
        REQUIRE(test_vector.capacity() == 100);
    }
}

TEST_CASE("Pushback", "[access][modify]")
{
    SECTION("pushback")
    {
        atl::vector<int> test_vector;
        test_vector.push_back(5);
        test_vector.push_back(10);

        REQUIRE(test_vector.size() == 2);
        REQUIRE(test_vector.capacity() == 10);

        REQUIRE(test_vector[0] == 5);
        REQUIRE(test_vector[1] == 10);

        for (int i = 0; i < 10; i++) {
            test_vector.push_back(i);
        }

        REQUIRE(test_vector.size() == 12);
        REQUIRE(test_vector.capacity() == 15);
        for (size_t i = 2; i < test_vector.size(); i++)
        {
            REQUIRE(test_vector[i] == i-2);
        }
    }
}
