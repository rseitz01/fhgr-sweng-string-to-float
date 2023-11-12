#include <stdio.h>
#include <stdlib.h>

#include "string_to_float.h"
#include <stdint.h>

double sign_exponent_mantissa_to_double(int sign, uint64_t exponent, uint64_t mantissa);

int main(void)
{
    //printf("%e\n", (double)9999999999999999);
    //printf("%.2f\n", string_to_float("0"));
    //printf("%.16f\n", sign_exponent_mantissa_to_double(0, 6, 1));

    string_to_float("123");

    printf("Hello\n");
    return 0;
}

