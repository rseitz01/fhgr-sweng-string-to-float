#include <errno.h>
#include <stdlib.h>

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
    snprintf(msg, 128, "(format '%s')", fmt);
    if(expect != expect) {
        /* IEEE standard: f != f -> true only for NaN */
        /* however, unity.h isn't the IEEE standard and the function does exactly what is says */
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(NAN, actual, msg);
        TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, "(errno)");
    } else if(v <= 9007199254740992) {
        double e = pow(10, round(log10(expect)) - 16);
        TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(e, fabs(expect - actual), msg);
        //TEST_ASSERT_EQUAL_DOUBLE_MESSAGE((double)v, actual, msg);
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
    test_int("ABC%zu ", v, NAN);
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
TEST_INT(9007199254740992);
TEST_INT(9007199254740993);





void test_double(char *fmt, char *v, char *m, double expect, int err_expect)
{
    char buf[128] = {0};
    snprintf(buf, 128, fmt, v, m);
    double actual = string_to_float(buf);
    char msg[128] = {0};
    snprintf(msg, 128, "(format '%s' -> expect %.17g is %.17g)", fmt, expect, actual);

    if(expect != expect) {
        /* IEEE standard: f != f -> true only for NaN */
        /* however, unity.h isn't the IEEE standard and the function does exactly what is says */
        TEST_ASSERT_NOT_EQUAL(0, err_expect);
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(NAN, actual, msg);
        TEST_ASSERT_EQUAL_MESSAGE(err_expect, errno, "(errno)");
    } else {
        TEST_ASSERT_EQUAL(0, err_expect);
        double e = pow(10, round(log10(expect)) - 10);
        TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(e, fabs(expect - actual), msg);
        TEST_ASSERT_EQUAL_MESSAGE(err_expect, errno, "(errno)");
    }
}

void test_double_pattern(char *v, char *m, double expect, int err_expect)
{
    test_double("%s.%s", v, m, expect, err_expect);
    test_double(" %s.%s", v, m, expect, err_expect);
    test_double("%s.%s ", v, m, expect, err_expect);
    test_double("	%s.%s", v, m, expect, err_expect);
    test_double("%s.%s	", v, m, expect, err_expect);
    test_double(" 	 %s.%s 	 ", v, m, expect, err_expect);
    test_double("%s.%s ABC", v, m, NAN, EINVAL);
    test_double("ABC%s.%s", v, m, NAN, EINVAL);
    test_double("ABC%s.%sABC", v, m, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", v, m, NAN, EINVAL);
}

#if 0
void test_double_0_001(void)
{
    test_double_pattern("0", "001", 0.001);
}
#endif

#define STR(X)  #X
#define XSTR(X) STR(X)

#define TEST_DOUBLE(V, M, E, EX) \
    void test_double_##V##_##M(void) { \
        test_double_pattern(STR(V), STR(M), E, EX); \
    }

TEST_DOUBLE(0, 1, 0.1, 0);
TEST_DOUBLE(0, 01, 0.01, 0);
TEST_DOUBLE(0, 001, 0.001, 0);
TEST_DOUBLE(0, 1234, 0.1234, 0);
TEST_DOUBLE(0, 12345, 0.12345, 0);
TEST_DOUBLE(0, 123456, 0.123456, 0);
TEST_DOUBLE(0, 1234567, 0.1234567, 0);
TEST_DOUBLE(0, 12345678, 0.12345678, 0);
TEST_DOUBLE(0, 123456789, 0.123456789, 0);
TEST_DOUBLE(900719925474099, 2, 900719925474099, 0);
TEST_DOUBLE(90071992547409, 92, 90071992547409.92, 0);
TEST_DOUBLE(9007199254740, 992, 9007199254740.992, 0);
TEST_DOUBLE(900719925474, 0992, 900719925474.0992, 0);
TEST_DOUBLE(90071992547, 40992, 90071992547.40992, 0);
TEST_DOUBLE(9007199254, 740992, 9007199254.740992, 0);
TEST_DOUBLE(900719925, 4740992, 900719925.4740992, 0);
TEST_DOUBLE(90071992, 54740992, 90071992.54740992, 0);
TEST_DOUBLE(9007199, 254740992, 9007199.254740992, 0);
TEST_DOUBLE(900719, 9254740992, 900719.9254740992, 0);
TEST_DOUBLE(90071, 99254740992, 90071.99254740992, 0);
TEST_DOUBLE(9007, 199254740992, 9007.199254740992, 0);
TEST_DOUBLE(900, 7199254740992, 900.7199254740992, 0);
TEST_DOUBLE(90, 07199254740992, 90.07199254740992, 0);
TEST_DOUBLE(9, 007199254740992, 9.007199254740992, 0);
TEST_DOUBLE(0,9007199254740992,0.9007199254740992, 0);
TEST_DOUBLE(900719925474099, 0, 900719925474099, 0);


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
    RUN_TEST(test_int_9007199254740992);
    RUN_TEST(test_int_9007199254740993);
    RUN_TEST(test_double_0_1);
    RUN_TEST(test_double_0_01);
    RUN_TEST(test_double_0_001);
    RUN_TEST(test_double_0_1234);
    RUN_TEST(test_double_0_12345);
    RUN_TEST(test_double_0_123456);
    RUN_TEST(test_double_0_1234567);
    RUN_TEST(test_double_0_12345678);
    RUN_TEST(test_double_0_123456789);

    RUN_TEST(test_double_900719925474099_2);
    RUN_TEST(test_double_90071992547409_92);
    RUN_TEST(test_double_9007199254740_992);
    RUN_TEST(test_double_900719925474_0992);
    RUN_TEST(test_double_90071992547_40992);
    RUN_TEST(test_double_9007199254_740992);
    RUN_TEST(test_double_900719925_4740992);
    RUN_TEST(test_double_90071992_54740992);
    RUN_TEST(test_double_9007199_254740992);
    RUN_TEST(test_double_900719_9254740992);
    RUN_TEST(test_double_90071_99254740992);
    RUN_TEST(test_double_9007_199254740992);
    RUN_TEST(test_double_900_7199254740992);
    RUN_TEST(test_double_90_07199254740992);
    RUN_TEST(test_double_9_007199254740992);
    RUN_TEST(test_double_0_9007199254740992);


    return UNITY_END();
}

