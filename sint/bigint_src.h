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
	BIGINT_DATA_TYPE* data;
};

inline int bigint_is_NaN(bigint *self)
{
    return self->data == NULL;
}

inline bigint* bigint_abs(bigint *self)
{
    self->size = self->size < 0? -self->size : self->size;
}

bigint* new_bigint_int(intmax_t num)
{
    bigint *res = (bigint *)malloc(sizeof(bigint));
    if (res == NULL) {
        return NULL;
    }

    res->size = num < 0 ? -1 : 1;
    res->data[0] = num < 0 ? -num : num;

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

static inline bigint* _realloc_bigint(bigint **self, BIGINT_DATA_TYPE size)
{
    assert(self && *self && size >= 1);

    bigint *temp = *self;
    *self = (bigint*)realloc(*self, sizeof(bigint) + sizeof(size_t) * size);
    if (*self == NULL) {
        free(temp);
        return NULL;
    }

    if (size > (BIGINT_DATA_TYPE)((*self)->size < 0? -(*self)->size : (*self)->size)) {
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

bigint* bigint_shrNum(bigint *self, BIGINT_DATA_TYPE other)
{
    assert(self && other && self->data);

    BIGINT_DATA_TYPE is_odd = 0;
    for (intmax_t i = self->size - 1; i >= 0; --i) {
        BIGINT_DATA_TYPE is_odd_temp = self->data[i] & 1;
        self->data[i] >>= 1;
        self->data[i] |= is_odd << (sizeof(BIGINT_DATA_TYPE) * 8 - 1);
        is_odd = is_odd_temp;
    }

    return self;
}