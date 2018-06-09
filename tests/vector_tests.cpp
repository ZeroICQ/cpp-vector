#include "catch.hpp"
#include "vector.h"
#include <memory>
#include <cstring>

template <class T, class U, class = typename T::iterator, class = typename U::iterator>
bool is_same(const T& a, const U& s)
{
    if (s.size() != a.size()) {
        return false;
    }

    for (typename atl::vector<T>::size_type i = 0; i < a.size(); i++) {
        if (a[i] != s[i]) {
            return false;
        }
    }

    return true;
}

class NotIntegralType
{
public:
    NotIntegralType() : large_field_a(new int[100]), large_field_b(new int[100]) {}
    NotIntegralType(NotIntegralType&& a)
    {
        large_field_a = a.large_field_a;
        large_field_b = a.large_field_b;
        a.large_field_a = nullptr;
        a.large_field_b = nullptr;
    }

    NotIntegralType(const NotIntegralType& other) : NotIntegralType()
    {
        std::memcpy(large_field_a, other.large_field_a, 100 * ( sizeof(int)));
        std::memcpy(large_field_b, other.large_field_b, 100 * ( sizeof(int)));
    }

    ~NotIntegralType() {
        delete[] large_field_a;
        delete[] large_field_b;
    }

    int* large_field_a;
    int* large_field_b;
};

TEST_CASE("Constructors", "[create]")
{
    SECTION("vector(size_type n)")
    {
        atl::vector<double> test_vector(500);
        REQUIRE(test_vector.capacity() == 500);
        REQUIRE(test_vector.size() == 500);
        REQUIRE_FALSE(test_vector.empty());
    }

    SECTION("vector(size_type 0)")
    {
        atl::vector<bool> test_vector(0);
        REQUIRE(test_vector.capacity() == 0);
        REQUIRE(test_vector.size() == 0);
        REQUIRE(test_vector.empty());
    }

    SECTION("construct with iterators")
    {
        std::vector<int> std_vec = {0, 10, 132, 228};
        atl::vector<int> test_vec(std_vec.begin(), std_vec.end());
        REQUIRE(test_vec.capacity() == std_vec.size());

        REQUIRE(is_same(test_vec, std_vec));
    }

    SECTION("copy constuctor without allocator")
    {
        atl::vector<NotIntegralType> test_vector_a(10);
        test_vector_a[4].large_field_a[4] = 127;

        atl::vector<NotIntegralType> test_vector_b(test_vector_a);

        test_vector_a[4].large_field_a[4] = 128;
        REQUIRE(test_vector_b[4].large_field_a[4] == 127);
    }

    SECTION("move constructor")
    {
        //todo: write a better test
        atl::vector<int> test_vector_a(atl::vector<int>(15, 12));
        REQUIRE(test_vector_a[5] == 12);
    }

    SECTION("initializer_list constructors")
    {
        atl::vector<int> test_vector = {10, 12, 13, 199821};
        REQUIRE(test_vector[0] == 10);
        REQUIRE(test_vector[1] == 12);
        REQUIRE(test_vector[2] == 13);
        REQUIRE(test_vector[3] == 199821);

        REQUIRE(test_vector.size() == 4);
        REQUIRE(test_vector.capacity() == 4);
    }
}

TEST_CASE("Access", "[access]")
{
    SECTION(".at(pos)")
    {
        atl::vector<int> test_vector(10);
        int val = test_vector.at(5);
        REQUIRE(val == 0);
        REQUIRE_THROWS_AS(test_vector.at(11), std::out_of_range);
        REQUIRE_THROWS_AS(test_vector.at(-1), std::out_of_range);
    }

    SECTION("assign(n, elem)")
    {
        atl::vector<int> test_vector(10, 8);

        test_vector.assign(11, 272);

        REQUIRE(test_vector.size() == 11);
        REQUIRE(test_vector.capacity() == 11);

        for (auto& val : test_vector) {
            REQUIRE(val == 272);
        }

    }

    SECTION("assign Iterators")
    {
        atl::vector<int> test_vector(10, 8);
        std::vector<int> std_vector = {8, 9, 10, 11, 12, 78099090, -288};

        test_vector.assign(std_vector.begin(), std_vector.end());

        REQUIRE(test_vector.size() == std_vector.size());

        REQUIRE(is_same(test_vector, std_vector));

    }

    SECTION("assign initializer_list")
    {
        atl::vector<int> test_vector(10, 8);
        test_vector.assign({100, 200});

        REQUIRE(test_vector[0] == 100);
        REQUIRE(test_vector[1] == 200);

    }
}

TEST_CASE("Resize", "[access]")
{
    SECTION(".reserve")
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

        test_vector.resize(500, 9);

        test_vector.emplace_back(2);
        REQUIRE(test_vector.back() == 2);

    }

}

TEST_CASE("Operators")
{
    SECTION("copy assignment = ")
    {
        atl::vector<int> test_vector_a;
        atl::vector<int> test_vector_b;
        test_vector_a.push_back(5);
        test_vector_a.push_back(10);

        test_vector_b = test_vector_a;

        REQUIRE(is_same(test_vector_a, test_vector_b));

        test_vector_a[1] = 500;
        REQUIRE_FALSE(is_same(test_vector_a, test_vector_b));

    }

    SECTION(" = initializer_list")
    {
        atl::vector<std::string> test_vector = {"kek", "mek", "rek"};
        test_vector = {"rofl", "memes"};

        REQUIRE(test_vector.size() == 2);
        REQUIRE(test_vector[0] == "rofl");
        REQUIRE(test_vector[1] == "memes");
    }
}
