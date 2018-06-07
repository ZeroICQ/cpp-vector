#include "vector.h"
int main (int argc, char *argv[])
{
    atl::vector<double> test_vector(500);

    auto kek = test_vector[100];

    return EXIT_SUCCESS;
}