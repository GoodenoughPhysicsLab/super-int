#pragma once

#include <stdint.h>

#ifndef __BIG_INT__
#define __BIG_INT__
#endif // __BIG_INT__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct bigint bigint;

inline bigint* new_bigint_int(intmax_t);
bigint* new_bigint_str(const char *);
void del_bigint(bigint **);
void print_bigint(bigint *);

bigint* bigint_add(bigint *, bigint *);
bigint* bigint_sub(bigint *, bigint *);
bigint* bigint_mul(bigint *, bigint *);
bigint* bigint_div(bigint *, bigint *);

bigint* bigint_addNum(bigint *, uintmax_t);
bigint* bigint_subNum(bigint *, uintmax_t);
bigint* bigint_mulNum(bigint *, uintmax_t);
bigint* bigint_divNum(bigint *, uintmax_t);

bigint* bigint_lshift(bigint *, uintmax_t);
bigint* bigint_rshift(bigint *, uintmax_t);

bigint* bigint_not(bigint *);
bigint* bigint_or(bigint *, bigint *);
bigint* bigint_and(bigint *, bigint *);
bigint* bigint_xor(bigint *, bigint *);

#include "bigint_src.h"

#ifdef __cplusplus
}
#endif // __cpluscplus