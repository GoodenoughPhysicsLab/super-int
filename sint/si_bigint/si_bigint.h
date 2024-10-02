#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct si_bigint si_bigint;

si_bigint* si_bigint_new_from_num(intmax_t const);
si_bigint* si_bigint_new_from_str(char const *);
si_bigint* si_bigint_new_from_si_bigint(si_bigint const *);
void si_bigint_del(si_bigint *);
char const* si_bigint_to_str(si_bigint const *);
bool si_bigint_is_NaN(si_bigint const *);
void si_bigint_print(si_bigint const *);

void si_bigint_abs(si_bigint *);
void si_bigint_add(si_bigint **, si_bigint const *);
void si_bigint_add_num(si_bigint **, intmax_t const);
void si_bigint_sub(si_bigint **, si_bigint const *);
void si_bigint_sub_num(si_bigint **, intmax_t const);
void si_bigint_mul(si_bigint **, si_bigint const *);
void si_bigint_mul_num(si_bigint **, intmax_t const);
void si_bigint_div(si_bigint **, si_bigint const *);
void si_bigint_div_num(si_bigint **, intmax_t const);
void si_bigint_mod(si_bigint **, si_bigint const *);
void si_bigint_mod_num(si_bigint **, intmax_t const);
void si_bigint_pow(si_bigint **, si_bigint const *);
void si_bigint_pow_num(si_bigint **, intmax_t const);
void si_bigint_shl(si_bigint **);
void si_bigint_shr(si_bigint **);
void si_bigint_not(si_bigint **);
void si_bigint_and(si_bigint **, si_bigint const *);
void si_bigint_and_num(si_bigint **, intmax_t const);
void si_bigint_or(si_bigint **, si_bigint const *);
void si_bigint_or_num(si_bigint **, intmax_t const);
void si_bigint_xor(si_bigint **, si_bigint const *);
void si_bigint_xor_num(si_bigint **, intmax_t const);

bool si_bigint_eq(si_bigint const *, si_bigint const *);
bool si_bigint_eq_num(si_bigint const *, intmax_t const);
bool si_bigint_lt(si_bigint const *, si_bigint const *);
bool si_bigint_lt_num(si_bigint const *, intmax_t const);
bool si_bigint_gt(si_bigint const *, si_bigint const *);
bool si_bigint_gt_num(si_bigint const *, intmax_t const);

#ifdef __cplusplus
}
#endif // __cplusplus
