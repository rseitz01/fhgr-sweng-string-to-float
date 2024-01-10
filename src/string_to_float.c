#include <errno.h>
#include <fenv.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>

#include "string_to_float.h"

double string_to_float_s(char *str, size_t len)
{
    errno = 0;
    if(!str) {
        errno = EINVAL;
        return -INFINITY;
    }
    size_t i = 0, n_integer = 0, n_fraction = 0, n_exponent = 0;
    double integer = 0, integer_sign = 1;
    double fraction = 0;
    double exponent = 0, exponent_sign = 1;
    double result = 0;
    /* skip leading whitespace */
    while(i < len && isspace((int)str[i])) {
        i++;
    }
    /* get number of digits of int part */
    size_t nd = i;
    if(str[nd] == 'n' || str[nd] == 'N') {
        nd ++;
        if (str[nd] == 'a' || str[nd] == 'A') {
            nd++;
            if (str[nd] == 'n' || str[nd] == 'N') {
                return NAN;
            } else {
                goto error;
            }
        } else {
            goto error;
        }
    }

    /* integer sign */
    if(str[nd] == '+') {
        nd++;
        i++;
    } else if(str[nd] == '-') {
        nd++;
        i++;
        integer_sign *= -1;
    }
    /* skip integer digits */
    while(nd < len && isdigit((int)str[nd])) {
        nd++;
    }
    /* decode digit */
    double f = 1.0;
    for(size_t k = 0; k < nd - i; k++) {
        double digit = str[nd - k - 1] - '0';
        integer += (digit * f);
        n_integer++;
        f *= 10.0;
    }
    i = nd;
    /* check for comma */
    if(i < len && str[i] == '.') {
        i++; /* skip '.' */
        /* decode digits */
        double f = 0.1;
        while(i < len && isdigit((int)str[i])) {
            double digit = str[i] - '0';
            //printf("%s\n", &str[i]);
            fraction += (digit * f);
            n_fraction++;
            f *= 0.1;
            i++;
        }
    }
    /* check for exponent */
    if(i < len && (str[i] == 'e' || str[i] == 'E')) {
        /* required to have beginning as an integer or fraction! */
        if(!n_integer && !n_fraction) {
            goto error;
        }
        i++; /* skip 'e' */
        if(i < len) {
            if(str[i] == '+') {
                i++; /* skip '+' */
            } else if(str[i] == '-') {
                i++; /* skip '-' */
                exponent_sign *= -1;
            }
        }
        size_t ne = i;
        /* skip exponent digits */
        while(ne < len && isdigit((int)str[ne])) {
            ne++;
        }
        /* decode digit */
        double f = 1.0;
        for(size_t k = 0; k < ne - i; k++) {
            double digit = str[ne - k - 1] - '0';
            exponent += (digit * f);
            n_exponent++;
            f *= 10.0;
        }
        if(!n_exponent) {
            goto error;
        }
        i = ne;
    }
    /* skip trailing whitespace */
    while(i < len && isspace((int)str[i])) {
        i++;
    }
    /* error */
    if(errno || ((i < len) && str[i])) {
        goto error;
    }
    /* success */
    result = integer_sign * (integer + fraction) * pow(10, exponent_sign * exponent);
    //printf("%f = %f * (%f + %f) * pow(10, %f)\n", result, integer_sign, integer, fraction, exponent);
    return result;
    /* please agree with me that a goto error is nice to have :) */
error:
    errno = EINVAL;
    return NAN;
}

double string_to_float(char *str)
{
    if(!str) {
        errno = EINVAL;
        return -INFINITY;
    }
    size_t len = strlen(str);
    return string_to_float_s(str, len);
}



