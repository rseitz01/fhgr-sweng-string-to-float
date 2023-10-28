#include <errno.h>
#include <fenv.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>

#include "string_to_float.h"

double string_to_float_s(char *str, size_t len)
{
    errno = 0;
    if(!str) {
        errno = EINVAL;
        return -INFINITY;
    }
    size_t i = 0;
    size_t integer = 0;
    double result = 0;
    /* skip leading whitespace */
    while(str[i] && i < len && isspace((int)str[i])) {
        i++;
    }
    /* decode digit */
    while(str[i] && i < len && isdigit((int)str[i])) {
        int digit = str[i] - '0';
        integer = 10 * integer + digit;
        if((size_t)(double)integer != integer) {
            errno = ERANGE;
        }
        i++;
    }
    /* skip trailing whitespace */
    while(str[i] && i < len && isspace((int)str[i])) {
        i++;
    }
    /* error */
    if(errno || (i < len && str[i])) {
        goto error;
    }
    /* success */
    result = integer;
    return result;
    /* please agree with me that a goto error is nice to have :) */
error:
    if(i < len && str[i]) {
        errno = EINVAL;
    }
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



