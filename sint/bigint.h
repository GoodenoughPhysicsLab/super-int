#pragma once

#include <stdint.h>

#ifndef __BIG_INT__
#define __BIG_INT__
#endif // __BIG_INT__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef BIGINT_DATA_TYPE
#define BIGINT_DATA_TYPE uintmax_t
#endif

typedef struct bigint bigint;

bigint* new_bigint_int(intmax_t);
bigint* new_bigint_str(const char *);
void del_bigint(bigint **);
void print_bigint(bigint *);
inline int bigint_is_NaN(bigint *);

bigint* bigint_add(bigint *, bigint *);
bigint* bigint_sub(bigint *, bigint *);
bigint* bigint_mul(bigint *, bigint *);
bigint* bigint_div(bigint *, bigint *);
bigint* bigint_mod(bigint *, bigint *);

bigint* bigint_addNum(bigint *, BIGINT_DATA_TYPE);
bigint* bigint_subNum(bigint *, BIGINT_DATA_TYPE);
bigint* bigint_mulNum(bigint *, BIGINT_DATA_TYPE);
bigint* bigint_divNum(bigint *, BIGINT_DATA_TYPE);
bigint* bigint_modNum(bigint *, BIGINT_DATA_TYPE);

bigint* bigint_shl(bigint *, bigint *);
bigint* bigint_shr(bigint *, bigint *);

bigint* bigint_shlNum(bigint *, BIGINT_DATA_TYPE);
bigint* bigint_shrNum(bigint *, BIGINT_DATA_TYPE);

bigint* bigint_not(bigint *);
bigint* bigint_or(bigint *, bigint *);
bigint* bigint_and(bigint *, bigint *);
bigint* bigint_xor(bigint *, bigint *);

#include "bigint_src.h"

#ifdef __cplusplus
}
#endif // __cpluscplus