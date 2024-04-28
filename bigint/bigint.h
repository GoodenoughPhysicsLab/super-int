#pragma once

#include <stdint.h>

#ifndef __BIG_INT__
#define __BIG_INT__
#endif // __BIG_INT__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


struct bigint;
#define BIGINT struct bigint

inline BIGINT* new_bigint_int(intmax_t);
BIGINT* new_bigint_str(const char *);
void del_bigint(BIGINT **);
void print_bigint(BIGINT *);

BIGINT* bigint_add(BIGINT *, BIGINT *);
BIGINT* bigint_sub(BIGINT *, BIGINT *);
BIGINT* bigint_mul(BIGINT *, BIGINT *);
BIGINT* bigint_div(BIGINT *, BIGINT *);

BIGINT* bigint_addNum(BIGINT *, uintmax_t);
BIGINT* bigint_subNum(BIGINT *, uintmax_t);
BIGINT* bigint_mulNum(BIGINT *, uintmax_t);
BIGINT* bigint_divNum(BIGINT *, uintmax_t);

BIGINT* bigint_lshift(BIGINT *, uintmax_t);
BIGINT* bigint_rshift(BIGINT *, uintmax_t);

BIGINT* bigint_not(BIGINT *);
BIGINT* bigint_or(BIGINT *, BIGINT *);
BIGINT* bigint_and(BIGINT *, BIGINT *);
BIGINT* bigint_xor(BIGINT *, BIGINT *);

#undef BIGINT

#include "bigint_src.h"

#ifdef __cplusplus
}
#endif // __cpluscplus

#include "define.h"
#if defined(_MSC_VAR) && !BIGINT_IS_C11
#pragma warning(pop) // C4003
#endif