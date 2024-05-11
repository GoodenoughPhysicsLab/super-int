#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef BIGINT_USE_SIMD
#include <emmintrin.h>
#endif

/*
 *  .size : When bigint number negative, size is less than 0
 *  .data : When bigint number is NaN, data is NULL
 *
 *  add 1 to size if need
 */

struct bigint {
	intmax_t size;
#ifdef BIGINT_FLOAT
    double decimal;
#endif
	uintmax_t* data;
};

inline bigint* new_bigint_int(intmax_t num)
{
    bigint *res = (bigint *)malloc(sizeof(bigint));
    if (res == NULL) {
        return NULL;
    }

    res->size = num < 0 ? -1 : 1;

    res->data[0] = abs(num);

    return res;
}

bigint* new_bigint_str(const char *str)
{
    bigint *res = (bigint *)malloc(sizeof(bigint));

    return res;
}

inline void del_bigint(bigint **self)
{
    assert(self && *self);

    free(*self);
    *self = NULL;
}

static inline bigint* _realloc_bigint(bigint **self, uintmax_t size)
{
    assert(self && *self && size >= 1);

    bigint *temp = *self;
    *self = (bigint*)realloc(*self, sizeof(bigint) + sizeof(size_t) * size);
    if (*self == NULL) {
        free(temp);
        return NULL;
    }

    if (size > (uintmax_t)abs((*self)->size)) {
        memset(&(*self)->data[(*self)->size], 0, sizeof(size_t) * (size - (*self)->size));
    }
    (*self)->size = size;
    return *self;
}

bigint* bigint_add(bigint *self, bigint *other)
{
    return self;
}

bigint* bigint_sub(bigint *self, bigint *other)
{
    return self;
}

bigint* bigint_mul(bigint *self, bigint *other)
{
    return self;
}

bigint* bigint_div(bigint *self, bigint *other)
{
    return self;
}
