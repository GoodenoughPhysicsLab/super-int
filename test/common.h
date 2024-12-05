#pragma once

#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <si_bigint.h>

/* Create a new si_bigint from a multi-number
 */
si_bigint* new_si_bigint_from_multi_num_(si_len_type sign_and_len_arg, ...) {
    va_list args;
    va_start(args, sign_and_len_arg);

    size_t len_arg = (size_t)(sign_and_len_arg < 0 ? -sign_and_len_arg : sign_and_len_arg);
    si_bigint *res = (si_bigint*)malloc(sizeof(si_bigint) + len_arg * sizeof(si_data_type));
    if (res == NULL) {
        perror("test::BadAllocError");
        abort();
    }
    res->data = (si_data_type*)(&(res->data) + 1);
    res->len = sign_and_len_arg;
    for (size_t i = 0; i != len_arg; ++i) {
        res->data[i] = va_arg(args, si_data_type);
    }
    va_end(args);

    return res;
}
