#include <errno.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "string_to_float.h"

double string_to_float_s(char *str, size_t len)
{
    if(!str) {
        errno = EINVAL;
        return -INFINITY;
    }
    //errno = EBADMSG;
    double result = 0;
    size_t factor = 0;
    int dot = 0;
    for(size_t i = 0; i < len; i++) {
        int c = str[i];
        if(!c) break;
        if(isdigit(c)) {
        }
    }
    return result;
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



