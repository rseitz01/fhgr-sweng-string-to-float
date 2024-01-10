#include <errno.h>
#include <stdlib.h>

#include "unity.h"
#include "string_to_float.h"

#define PRECISION 15


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
    snprintf(msg, 128, "(format '%s' -> expect %.17g is %.17g)", fmt, expect, actual);
    if(expect != expect) {
        /* IEEE standard: f != f -> true only for NaN */
        /* however, unity.h isn't the IEEE standard and the function does exactly what is says */
        TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(NAN, actual, msg);
        TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, msg);
    } else {
        TEST_ASSERT_EQUAL_MESSAGE(0, errno, msg);
        double e = pow(10, round(log10(fabs(expect))) - PRECISION);
        TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(e, fabs(expect - actual), msg);
    }
}

void test_int_pattern(ssize_t v)
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
    /* prefix with negative */
    test_int("-%zu", v, -v);
    test_int(" -%zu", v, -v);
    test_int("-%zu ", v, -v);
    test_int(" -%zu ", v, -v);
    test_int("	-%zu", v, -v);
    test_int("-%zu	", v, -v);
    test_int(" 	 -%zu 	 ", v, -v);
    test_int("-%zuABC", v, NAN);
    test_int("ABC-%zu ", v, NAN);
    test_int("ABC-%zuABC", v, NAN);
    test_int(" ABC  -%zu  ABC ", v, NAN);
    /* prefix with positive */
    test_int("+%zu", v, v);
    test_int(" +%zu", v, v);
    test_int("+%zu ", v, v);
    test_int(" +%zu ", v, v);
    test_int("	+%zu", v, v);
    test_int("+%zu	", v, v);
    test_int(" 	 +%zu 	 ", v, v);
    test_int("+%zuABC", v, NAN);
    test_int("ABC+%zu ", v, NAN);
    test_int("ABC+%zuABC", v, NAN);
    test_int(" ABC  +%zu  ABC ", v, NAN);
}

#define TEST_INT(X) \
    void test_int_##X(void) { \
        test_int_pattern(X); \
    }

TEST_INT(0)
TEST_INT(1)
TEST_INT(2)
TEST_INT(3)
TEST_INT(4)
TEST_INT(5)
TEST_INT(6)
TEST_INT(7)
TEST_INT(8)
TEST_INT(9)
TEST_INT(123)
TEST_INT(1000)
TEST_INT(987654321)
TEST_INT(1234567890)
TEST_INT(999999999999999)
TEST_INT(9999999999999999)
TEST_INT(9007199254740992)
TEST_INT(9007199254740993)





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
        TEST_ASSERT_EQUAL_MESSAGE(err_expect, errno, "(errno)");
        TEST_ASSERT_EQUAL(0, err_expect);
        double e = pow(10, round(log10(fabs(expect))) - PRECISION);
        //printf("e = %.17f <= fabs(%f-%f) = %.17f\n", e, expect, actual, fabs(expect- actual));
        TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(e, fabs(expect - actual), msg);
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

    /* prefix with negative */
    test_double("-%s.%s", v, m, -expect, err_expect);
    test_double(" -%s.%s", v, m, -expect, err_expect);
    test_double("-%s.%s ", v, m, -expect, err_expect);
    test_double("	-%s.%s", v, m, -expect, err_expect);
    test_double("-%s.%s	", v, m, -expect, err_expect);
    test_double(" 	 -%s.%s 	 ", v, m, -expect, err_expect);
    test_double("-%s.%s ABC", v, m, NAN, EINVAL);
    test_double("ABC-%s.%s", v, m, NAN, EINVAL);
    test_double("ABC-%s.%sABC", v, m, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", v, m, NAN, EINVAL);

    /* prefix with positive */
    test_double("+%s.%s", v, m, expect, err_expect);
    test_double(" +%s.%s", v, m, expect, err_expect);
    test_double("+%s.%s ", v, m, expect, err_expect);
    test_double("	+%s.%s", v, m, expect, err_expect);
    test_double("+%s.%s	", v, m, expect, err_expect);
    test_double(" 	 +%s.%s 	 ", v, m, expect, err_expect);
    test_double("+%s.%s ABC", v, m, NAN, EINVAL);
    test_double("ABC+%s.%s", v, m, NAN, EINVAL);
    test_double("ABC+%s.%sABC", v, m, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", v, m, NAN, EINVAL);
}

#define STR(X)  #X
#define XSTR(X) STR(X)

#define TEST_DOUBLE(V, M, E, EX) \
    void test_double_##V##_##M(void) { \
        test_double_pattern(STR(V), STR(M), E, EX); \
    }

TEST_DOUBLE(0, 1, 0.1, 0)
TEST_DOUBLE(0, 01, 0.01, 0)
TEST_DOUBLE(0, 001, 0.001, 0)
TEST_DOUBLE(0, 1234, 0.1234, 0)
TEST_DOUBLE(0, 12345, 0.12345, 0)
TEST_DOUBLE(0, 123456, 0.123456, 0)
TEST_DOUBLE(0, 1234567, 0.1234567, 0)
TEST_DOUBLE(0, 12345678, 0.12345678, 0)
TEST_DOUBLE(0, 123456789, 0.123456789, 0)
TEST_DOUBLE(900719925474099, 2, 900719925474099, 0)
TEST_DOUBLE(90071992547409, 92, 90071992547409.92, 0)
TEST_DOUBLE(9007199254740, 992, 9007199254740.992, 0)
TEST_DOUBLE(900719925474, 0992, 900719925474.0992, 0)
TEST_DOUBLE(90071992547, 40992, 90071992547.40992, 0)
TEST_DOUBLE(9007199254, 740992, 9007199254.740992, 0)
TEST_DOUBLE(900719925, 4740992, 900719925.4740992, 0)
TEST_DOUBLE(90071992, 54740992, 90071992.54740992, 0)
TEST_DOUBLE(9007199, 254740992, 9007199.254740992, 0)
TEST_DOUBLE(900719, 9254740992, 900719.9254740992, 0)
TEST_DOUBLE(90071, 99254740992, 90071.99254740992, 0)
TEST_DOUBLE(9007, 199254740992, 9007.199254740992, 0)
TEST_DOUBLE(900, 7199254740992, 900.7199254740992, 0)
TEST_DOUBLE(90, 07199254740992, 90.07199254740992, 0)
TEST_DOUBLE(9, 007199254740992, 9.007199254740992, 0)
TEST_DOUBLE(0,9007199254740992,0.9007199254740992, 0)
TEST_DOUBLE(900719925474099, 0, 900719925474099, 0)

void test_exponent_pattern(char *b, char *e, double expect, int err_expect)
{
    test_double("%se%s", b, e, expect, err_expect);
    test_double(" %se%s", b, e, expect, err_expect);
    test_double("%se%s ", b, e, expect, err_expect);
    test_double("	%se%s", b, e, expect, err_expect);
    test_double("%se%s	", b, e, expect, err_expect);
    test_double(" 	 %se%s 	 ", b, e, expect, err_expect);
    test_double("%se%s ABC", b, e, NAN, EINVAL);
    test_double("ABC%se%s", b, e, NAN, EINVAL);
    test_double("ABC%se%sABC", b, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", b, e, NAN, EINVAL);

    test_double("%sE%s", b, e, expect, err_expect);
    test_double(" %sE%s", b, e, expect, err_expect);
    test_double("%sE%s ", b, e, expect, err_expect);
    test_double("	%sE%s", b, e, expect, err_expect);
    test_double("%sE%s	", b, e, expect, err_expect);
    test_double(" 	 %sE%s 	 ", b, e, expect, err_expect);
    test_double("%sE%s ABC", b, e, NAN, EINVAL);
    test_double("ABC%sE%s", b, e, NAN, EINVAL);
    test_double("ABC%sE%sABC", b, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", b, e, NAN, EINVAL);
}

#define TEST_EXPONENT(V, M, E, EX) \
    void test_##V##_exponent_##M(void) { \
        test_exponent_pattern(STR(V), STR(M), E, EX); \
    }

void test_exponent_minus_pattern(char *b, char *e, double expect, int err_expect)
{
    test_double("%se-%s", b, e, expect, err_expect);
    test_double(" %se-%s", b, e, expect, err_expect);
    test_double("%se-%s ", b, e, expect, err_expect);
    test_double("	%se-%s", b, e, expect, err_expect);
    test_double("%se-%s	", b, e, expect, err_expect);
    test_double(" 	 %se-%s 	 ", b, e, expect, err_expect);
    test_double("%se-%s ABC", b, e, NAN, EINVAL);
    test_double("ABC%se-%s", b, e, NAN, EINVAL);
    test_double("ABC%se-%sABC", b, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", b, e, NAN, EINVAL);

    test_double("%sE-%s", b, e, expect, err_expect);
    test_double(" %sE-%s", b, e, expect, err_expect);
    test_double("%sE-%s ", b, e, expect, err_expect);
    test_double("	%sE-%s", b, e, expect, err_expect);
    test_double("%sE-%s	", b, e, expect, err_expect);
    test_double(" 	 %sE-%s 	 ", b, e, expect, err_expect);
    test_double("%sE-%s ABC", b, e, NAN, EINVAL);
    test_double("ABC%sE-%s", b, e, NAN, EINVAL);
    test_double("ABC%sE-%sABC", b, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", b, e, NAN, EINVAL);
}


TEST_EXPONENT(0, 1, 0, 0)
TEST_EXPONENT(0, 01, 0, 0)
TEST_EXPONENT(0, 001, 0, 0)
TEST_EXPONENT(1, 0, 1e0, 0)
TEST_EXPONENT(1, 1, 1e1, 0)
TEST_EXPONENT(1, 2, 1e2, 0)
TEST_EXPONENT(1, 6, 1e6, 0)
TEST_EXPONENT(1, 100, 1e100, 0)
TEST_EXPONENT(100, 0, 100e0, 0)
TEST_EXPONENT(100, 1, 100e1, 0)
TEST_EXPONENT(100, 2, 100e2, 0)
TEST_EXPONENT(100, 6, 100e6, 0)
TEST_EXPONENT(100, 100, 100e100, 0)

#define TEST_EXPONENT_MINUS(V, M, E, EX) \
    void test_##V##_exponent_minus_##M(void) { \
        test_exponent_minus_pattern(STR(V), STR(M), E, EX); \
    }

TEST_EXPONENT_MINUS(0, 1, 0, 0)
TEST_EXPONENT_MINUS(0, 01, 0, 0)
TEST_EXPONENT_MINUS(0, 001, 0, 0)
TEST_EXPONENT_MINUS(1, 0, 1e-0, 0)
TEST_EXPONENT_MINUS(1, 1, 1e-1, 0)
TEST_EXPONENT_MINUS(1, 2, 1e-2, 0)
TEST_EXPONENT_MINUS(1, 6, 1e-6, 0)
TEST_EXPONENT_MINUS(1, 100, 1e-100, 0)
TEST_EXPONENT_MINUS(100, 0, 100e-0, 0)
TEST_EXPONENT_MINUS(100, 1, 100e-1, 0)
TEST_EXPONENT_MINUS(100, 2, 100e-2, 0)
TEST_EXPONENT_MINUS(100, 6, 100e-6, 0)
TEST_EXPONENT_MINUS(100, 100, 100e-100, 0)

void test_mixed_pattern(char *m, char *e, double expect, double expect2, int err_expect)
{
    test_double("%se%s", m, e, expect, err_expect);
    test_double(" %se%s", m, e, expect, err_expect);
    test_double("%se%s ", m, e, expect, err_expect);
    test_double("	%se%s", m, e, expect, err_expect);
    test_double("%se%s	", m, e, expect, err_expect);
    test_double(" 	 %se%s 	 ", m, e, expect, err_expect);
    test_double("%se%s ABC", m, e, NAN, EINVAL);
    test_double("ABC%se%s", m, e, NAN, EINVAL);
    test_double("ABC%se%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("%sE%s", m, e, expect, err_expect);
    test_double(" %sE%s", m, e, expect, err_expect);
    test_double("%sE%s ", m, e, expect, err_expect);
    test_double("	%sE%s", m, e, expect, err_expect);
    test_double("%sE%s	", m, e, expect, err_expect);
    test_double(" 	 %sE%s 	 ", m, e, expect, err_expect);
    test_double("%sE%s ABC", m, e, NAN, EINVAL);
    test_double("ABC%sE%s", m, e, NAN, EINVAL);
    test_double("ABC%sE%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("%se-%s", m, e, expect2, err_expect);
    test_double(" %se-%s", m, e, expect2, err_expect);
    test_double("%se-%s ", m, e, expect2, err_expect);
    test_double("	%se-%s", m, e, expect2, err_expect);
    test_double("%se-%s	", m, e, expect2, err_expect);
    test_double(" 	 %se-%s 	 ", m, e, expect2, err_expect);
    test_double("%se-%s ABC", m, e, NAN, EINVAL);
    test_double("ABC%se-%s", m, e, NAN, EINVAL);
    test_double("ABC%se-%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("%sE-%s", m, e, expect2, err_expect);
    test_double(" %sE-%s", m, e, expect2, err_expect);
    test_double("%sE-%s ", m, e, expect2, err_expect);
    test_double("	%sE-%s", m, e, expect2, err_expect);
    test_double("%sE-%s	", m, e, expect2, err_expect);
    test_double(" 	 %sE-%s 	 ", m, e, expect2, err_expect);
    test_double("%sE-%s ABC", m, e, NAN, EINVAL);
    test_double("ABC%sE-%s", m, e, NAN, EINVAL);
    test_double("ABC%sE-%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("%se+%s", m, e, expect, err_expect);
    test_double(" %se+%s", m, e, expect, err_expect);
    test_double("%se+%s ", m, e, expect, err_expect);
    test_double("	%se+%s", m, e, expect, err_expect);
    test_double("%se+%s	", m, e, expect, err_expect);
    test_double(" 	 %se+%s 	 ", m, e, expect, err_expect);
    test_double("%se+%s ABC", m, e, NAN, EINVAL);
    test_double("ABC%se+%s", m, e, NAN, EINVAL);
    test_double("ABC%se+%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("%sE+%s", m, e, expect, err_expect);
    test_double(" %sE+%s", m, e, expect, err_expect);
    test_double("%sE+%s ", m, e, expect, err_expect);
    test_double("	%sE+%s", m, e, expect, err_expect);
    test_double("%sE+%s	", m, e, expect, err_expect);
    test_double(" 	 %sE+%s 	 ", m, e, expect, err_expect);
    test_double("%sE+%s ABC", m, e, NAN, EINVAL);
    test_double("ABC%sE+%s", m, e, NAN, EINVAL);
    test_double("ABC%sE+%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    /* NOW PREFIX WITH NEGATIVE SIGN */
    test_double("-%se%s", m, e, -expect, err_expect);
    test_double(" -%se%s", m, e, -expect, err_expect);
    test_double("-%se%s ", m, e, -expect, err_expect);
    test_double("	-%se%s", m, e, -expect, err_expect);
    test_double("-%se%s	", m, e, -expect, err_expect);
    test_double(" 	 -%se%s 	 ", m, e, -expect, err_expect);
    test_double("-%se%s ABC", m, e, NAN, EINVAL);
    test_double("ABC-%se%s", m, e, NAN, EINVAL);
    test_double("ABC-%se%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("-%sE%s", m, e, -expect, err_expect);
    test_double(" -%sE%s", m, e, -expect, err_expect);
    test_double("-%sE%s ", m, e, -expect, err_expect);
    test_double("	-%sE%s", m, e, -expect, err_expect);
    test_double("-%sE%s	", m, e, -expect, err_expect);
    test_double(" 	 -%sE%s 	 ", m, e, -expect, err_expect);
    test_double("-%sE%s ABC", m, e, NAN, EINVAL);
    test_double("ABC-%sE%s", m, e, NAN, EINVAL);
    test_double("ABC-%sE%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("-%se-%s", m, e, -expect2, err_expect);
    test_double(" -%se-%s", m, e, -expect2, err_expect);
    test_double("-%se-%s ", m, e, -expect2, err_expect);
    test_double("	-%se-%s", m, e, -expect2, err_expect);
    test_double("-%se-%s	", m, e, -expect2, err_expect);
    test_double(" 	 -%se-%s 	 ", m, e, -expect2, err_expect);
    test_double("-%se-%s ABC", m, e, NAN, EINVAL);
    test_double("ABC-%se-%s", m, e, NAN, EINVAL);
    test_double("ABC-%se-%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("-%sE-%s", m, e, -expect2, err_expect);
    test_double(" -%sE-%s", m, e, -expect2, err_expect);
    test_double("-%sE-%s ", m, e, -expect2, err_expect);
    test_double("	-%sE-%s", m, e, -expect2, err_expect);
    test_double("-%sE-%s	", m, e, -expect2, err_expect);
    test_double(" 	 -%sE-%s 	 ", m, e, -expect2, err_expect);
    test_double("-%sE-%s ABC", m, e, NAN, EINVAL);
    test_double("ABC-%sE-%s", m, e, NAN, EINVAL);
    test_double("ABC-%sE-%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("-%se+%s", m, e, -expect, err_expect);
    test_double(" -%se+%s", m, e, -expect, err_expect);
    test_double("-%se+%s ", m, e, -expect, err_expect);
    test_double("	-%se+%s", m, e, -expect, err_expect);
    test_double("-%se+%s	", m, e, -expect, err_expect);
    test_double(" 	 -%se+%s 	 ", m, e, -expect, err_expect);
    test_double("-%se+%s ABC", m, e, NAN, EINVAL);
    test_double("ABC-%se+%s", m, e, NAN, EINVAL);
    test_double("ABC-%se+%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("-%sE+%s", m, e, -expect, err_expect);
    test_double(" -%sE+%s", m, e, -expect, err_expect);
    test_double("-%sE+%s ", m, e, -expect, err_expect);
    test_double("	-%sE+%s", m, e, -expect, err_expect);
    test_double("-%sE+%s	", m, e, -expect, err_expect);
    test_double(" 	 -%sE+%s 	 ", m, e, -expect, err_expect);
    test_double("-%sE+%s ABC", m, e, NAN, EINVAL);
    test_double("ABC-%sE+%s", m, e, NAN, EINVAL);
    test_double("ABC-%sE+%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    /* NOW PREFIX WITH POSITIVE SIGN */
    test_double("+%se%s", m, e, +expect, err_expect);
    test_double(" +%se%s", m, e, +expect, err_expect);
    test_double("+%se%s ", m, e, +expect, err_expect);
    test_double("	+%se%s", m, e, +expect, err_expect);
    test_double("+%se%s	", m, e, +expect, err_expect);
    test_double(" 	 +%se%s 	 ", m, e, +expect, err_expect);
    test_double("+%se%s ABC", m, e, NAN, EINVAL);
    test_double("ABC+%se%s", m, e, NAN, EINVAL);
    test_double("ABC+%se%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("+%sE%s", m, e, +expect, err_expect);
    test_double(" +%sE%s", m, e, +expect, err_expect);
    test_double("+%sE%s ", m, e, +expect, err_expect);
    test_double("	+%sE%s", m, e, +expect, err_expect);
    test_double("+%sE%s	", m, e, +expect, err_expect);
    test_double(" 	 +%sE%s 	 ", m, e, +expect, err_expect);
    test_double("+%sE%s ABC", m, e, NAN, EINVAL);
    test_double("ABC+%sE%s", m, e, NAN, EINVAL);
    test_double("ABC+%sE%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("+%se-%s", m, e, +expect2, err_expect);
    test_double(" +%se-%s", m, e, +expect2, err_expect);
    test_double("+%se-%s ", m, e, +expect2, err_expect);
    test_double("	+%se-%s", m, e, +expect2, err_expect);
    test_double("+%se-%s	", m, e, +expect2, err_expect);
    test_double(" 	 +%se-%s 	 ", m, e, +expect2, err_expect);
    test_double("+%se-%s ABC", m, e, NAN, EINVAL);
    test_double("ABC+%se-%s", m, e, NAN, EINVAL);
    test_double("ABC+%se-%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("+%sE-%s", m, e, +expect2, err_expect);
    test_double(" +%sE-%s", m, e, +expect2, err_expect);
    test_double("+%sE-%s ", m, e, +expect2, err_expect);
    test_double("	+%sE-%s", m, e, +expect2, err_expect);
    test_double("+%sE-%s	", m, e, +expect2, err_expect);
    test_double(" 	 +%sE-%s 	 ", m, e, +expect2, err_expect);
    test_double("+%sE-%s ABC", m, e, NAN, EINVAL);
    test_double("ABC+%sE-%s", m, e, NAN, EINVAL);
    test_double("ABC+%sE-%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("+%se+%s", m, e, +expect, err_expect);
    test_double(" +%se+%s", m, e, +expect, err_expect);
    test_double("+%se+%s ", m, e, +expect, err_expect);
    test_double("	+%se+%s", m, e, +expect, err_expect);
    test_double("+%se+%s	", m, e, +expect, err_expect);
    test_double(" 	 +%se+%s 	 ", m, e, +expect, err_expect);
    test_double("+%se+%s ABC", m, e, NAN, EINVAL);
    test_double("ABC+%se+%s", m, e, NAN, EINVAL);
    test_double("ABC+%se+%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);

    test_double("+%sE+%s", m, e, +expect, err_expect);
    test_double(" +%sE+%s", m, e, +expect, err_expect);
    test_double("+%sE+%s ", m, e, +expect, err_expect);
    test_double("	+%sE+%s", m, e, +expect, err_expect);
    test_double("+%sE+%s	", m, e, +expect, err_expect);
    test_double(" 	 +%sE+%s 	 ", m, e, +expect, err_expect);
    test_double("+%sE+%s ABC", m, e, NAN, EINVAL);
    test_double("ABC+%sE+%s", m, e, NAN, EINVAL);
    test_double("ABC+%sE+%sABC", m, e, NAN, EINVAL);
    test_double(" ABC  %zu  ABC ", m, e, NAN, EINVAL);
}

#define TEST_MIXED(V, B, M, E, E2, EX) \
    void test_mixed_##V##_##B##_exponent_##M(void) { \
        test_mixed_pattern(STR(V) "." STR(B), STR(M), E, E2, EX); \
    }

TEST_MIXED(0, 3255, 1, 0.3255e1, 0.3255e-1, 0)
TEST_MIXED(0, 125, 01, 0.125e01, 0.125e-01, 0)
TEST_MIXED(0, 0953, 001, 0.0953e001, 0.0953e-001, 0)
TEST_MIXED(1, 9350, 0, 1.9350e0, 1.9350e-0, 0)
TEST_MIXED(1, 0235, 1, 1.0235e1, 1.0235e-1, 0)
TEST_MIXED(1, 25125, 2, 1.25125e2, 1.25125e-2, 0)
TEST_MIXED(1, 36431, 6, 1.36431e6, 1.36431e-6, 0)
TEST_MIXED(1, 3523, 100, 1.3523e100, 1.3523e-100, 0)
TEST_MIXED(100, 12345, 0, 100.12345e0, 100.12345e-0, 0)
TEST_MIXED(100, 345, 1, 100.345e1, 100.345e-1, 0)
TEST_MIXED(100, 643, 2, 100.643e2, 100.643e-2, 0)
TEST_MIXED(100, 34634, 6, 100.34634e6, 100.34634e-6, 0)
TEST_MIXED(100, 235, 100, 100.235e100, 100.235e-100, 0)

void test_nan(void)
{
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("nan"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("Nan"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("NAn"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("NAN"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("NaN"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("nAn"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("nAN"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN(string_to_float("naN"));
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
}

void test_incomplete(void)
{
    TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(12., string_to_float("12."), "testing 12.");
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(.12, string_to_float(".12"), "testing .12");
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN_MESSAGE(string_to_float(".12."), "testing .12.");
    TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN_MESSAGE(string_to_float("1e"), "testing 1e");
    TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, "(errno)");
    TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(1.e12, string_to_float("1.e12"), "testing 1.e12");
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(.1e12, string_to_float(".1e12"), "testing .1e12");
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN_MESSAGE(string_to_float("e21"), "testing e21");
    TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, "(errno)");
    TEST_ASSERT_LESS_OR_EQUAL_DOUBLE_MESSAGE(-.1e12, string_to_float("-.1e12"), "testing -.1e12");
    TEST_ASSERT_EQUAL_MESSAGE(0, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN_MESSAGE(string_to_float("-.1e-"), "testing -.1e-");
    TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, "(errno)");
    TEST_ASSERT_FLOAT_IS_NAN_MESSAGE(string_to_float("1d"), "testing 1d");
    TEST_ASSERT_EQUAL_MESSAGE(EINVAL, errno, "(errno)");
}

int main(void)
{
    UNITY_BEGIN();

    /* integer inputs */
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
    RUN_TEST(test_int_1000);
    RUN_TEST(test_int_987654321);
    RUN_TEST(test_int_1234567890);
    RUN_TEST(test_int_999999999999999);
    RUN_TEST(test_int_9999999999999999);
    RUN_TEST(test_int_9007199254740992);
    RUN_TEST(test_int_9007199254740993);

    /* floating inputs NUM1.NUM2 */
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

    /* exponent test NUM1eNUM2 */
    RUN_TEST(test_0_exponent_1);
    RUN_TEST(test_0_exponent_01);
    RUN_TEST(test_0_exponent_001);
    RUN_TEST(test_1_exponent_1);
    RUN_TEST(test_1_exponent_2);
    RUN_TEST(test_1_exponent_6);
    RUN_TEST(test_1_exponent_100);
    RUN_TEST(test_100_exponent_1);
    RUN_TEST(test_100_exponent_2);
    RUN_TEST(test_100_exponent_6);
    RUN_TEST(test_100_exponent_100);

    /* negative exponent test NUM1e-NUM2 */
    RUN_TEST(test_0_exponent_minus_1);
    RUN_TEST(test_0_exponent_minus_01);
    RUN_TEST(test_0_exponent_minus_001);
    RUN_TEST(test_1_exponent_minus_1);
    RUN_TEST(test_1_exponent_minus_2);
    RUN_TEST(test_1_exponent_minus_6);
    RUN_TEST(test_1_exponent_minus_100);
    RUN_TEST(test_100_exponent_minus_1);
    RUN_TEST(test_100_exponent_minus_2);
    RUN_TEST(test_100_exponent_minus_6);
    RUN_TEST(test_100_exponent_minus_100);

    /* mixed test NUM1.NUM2e+-NUM3 */
    RUN_TEST(test_mixed_0_3255_exponent_1);
    RUN_TEST(test_mixed_0_125_exponent_01);
    RUN_TEST(test_mixed_0_0953_exponent_001);
    RUN_TEST(test_mixed_1_9350_exponent_0);
    RUN_TEST(test_mixed_1_0235_exponent_1);
    RUN_TEST(test_mixed_1_25125_exponent_2);
    RUN_TEST(test_mixed_1_36431_exponent_6);
    RUN_TEST(test_mixed_1_3523_exponent_100);
    RUN_TEST(test_mixed_100_12345_exponent_0);
    RUN_TEST(test_mixed_100_345_exponent_1);
    RUN_TEST(test_mixed_100_643_exponent_2);
    RUN_TEST(test_mixed_100_34634_exponent_6);
    RUN_TEST(test_mixed_100_235_exponent_100);

    /* other tests */
    RUN_TEST(test_nan);
    RUN_TEST(test_incomplete);

    return UNITY_END();
}

