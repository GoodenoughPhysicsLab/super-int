#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef BIGINT_USE_SIMD
#include <emmintrin.h>
#endif

typedef struct bigint {
	//int8_t sign;
	intmax_t size; // When negative, size is less than 0
#ifdef BIGINT_FLOAT
    double decimal;
#endif
	uintmax_t data[1];
} bigint;

inline uintmax_t _abs_intmax_t(intmax_t num)
{
    if (num >= 0) {
        return num;
    }
    else {
        return -num;
    }
}

inline bigint* new_bigint_int(intmax_t num)
{
    bigint *res = (bigint *)malloc(sizeof(bigint));
    if (res == NULL) {
        return NULL;
    }

    if (num < 0) {
        res->size = -1;
    } else {
        res->size = 1;
    }

    res->data[0] = _abs_intmax_t(num);

    return res;
}

#if 0
bigint* new_bigint_str(const char *str)
{
    //
}
#endif

void del_bigint(bigint **self)
{
    assert(self && *self);

    free(*self);
    *self = NULL;
}

bigint* _realloc_bigint(bigint **self, size_t size)
{
    assert(self && *self && size >= 1);

    bigint *temp = *self;
    *self = (bigint*)realloc(*self, sizeof(bigint) + sizeof(size_t) * size);
    if (*self == NULL) {
        free(temp);
        return NULL;
    }

    if (size > _abs_intmax_t((*self)->size)) {
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
