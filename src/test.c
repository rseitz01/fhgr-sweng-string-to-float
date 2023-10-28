#include "unity.h"
#include "string_to_float.h"



void setUp(void)
{
}



void tearDown(void)
{
}



void test_int()
{
    TEST_ASSERT_EQUAL_DOUBLE(0, string_to_float("0"));
}



void test_even_numbers_no_prime()
{
    //TEST_ASSERT_FALSE(is_prime(4));
    //TEST_ASSERT_FALSE(is_prime(6));
    //TEST_ASSERT_FALSE(is_prime(18));
}



void test_large_mersenne_primes()
{
    //TEST_ASSERT(is_prime((1 << 13) - 1));
    //TEST_ASSERT(is_prime((1 << 17) - 1));
    //TEST_ASSERT(is_prime((1 << 19) - 1));
}



int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_int);
    RUN_TEST(test_even_numbers_no_prime);
    RUN_TEST(test_large_mersenne_primes);
    return UNITY_END();
}

