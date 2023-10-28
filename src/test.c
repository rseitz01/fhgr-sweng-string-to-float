#include <errno.h>

#include "unity.h"
#include "string_to_float.h"



void setUp(void)
{
}



void tearDown(void)
{
}



void test_int(char *fmt, size_t v, double expect)
{
    char buf[128] = {0};
    snprintf(buf, 128, fmt, v);
    double actual = string_to_float(buf);
    char msg[128] = {0};
    snprintf(msg, 128, "Format '%s'", fmt);
    if(expect != expect) {
        /* IEEE standard: f != f -> true only for NaN */
        /* however, unity.h isn't the IEEE standard and the function does exactly what is says */
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(NAN, actual, msg);
        TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, "(errno)");
    } else if(v <= 999999999999999) {
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE((double)v, actual, msg);
        TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    } else {
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(NAN, actual, msg);
        TEST_ASSERT_EQUAL_MESSAGE(ERANGE, errno, "(errno)");
    }
}

void test_int_pattern(size_t v)
{
    test_int("%zu", v, v);
    test_int(" %zu", v, v);
    test_int("%zu ", v, v);
    test_int(" %zu ", v, v);
    test_int("	%zu", v, v);
    test_int("%zu	", v, v);
    test_int(" 	 %zu 	 ", v, v);
    test_int("%zuABC", v, NAN);
    test_int("%zuABC ", v, NAN);
    test_int("ABC%zuABC", v, NAN);
    test_int(" ABC  %zu  ABC ", v, NAN);
}

#define TEST_INT(X) \
    void test_int_##X(void) { \
        test_int_pattern(X); \
    }

TEST_INT(0);
TEST_INT(1);
TEST_INT(2);
TEST_INT(3);
TEST_INT(4);
TEST_INT(5);
TEST_INT(6);
TEST_INT(7);
TEST_INT(8);
TEST_INT(9);
TEST_INT(123);
TEST_INT(987654321);
TEST_INT(1234567890);
TEST_INT(999999999999999);
TEST_INT(9999999999999999);

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_int_0);
    RUN_TEST(test_int_1);
    RUN_TEST(test_int_2);
    RUN_TEST(test_int_3);
    RUN_TEST(test_int_4);
    RUN_TEST(test_int_5);
    RUN_TEST(test_int_6);
    RUN_TEST(test_int_7);
    RUN_TEST(test_int_8);
    RUN_TEST(test_int_9);
    RUN_TEST(test_int_123);
    RUN_TEST(test_int_123);
    RUN_TEST(test_int_987654321);
    RUN_TEST(test_int_1234567890);
    RUN_TEST(test_int_999999999999999);
    RUN_TEST(test_int_9999999999999999);
    return UNITY_END();
}

