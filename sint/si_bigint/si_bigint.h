#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef intmax_t len_type_;
typedef uintmax_t data_type_;

typedef struct si_bigint {
    /* if si_bigint is negative, len < 0, else len > 0
     *
     * if si_bigint is NaN, len == 0
     *
     * if si_bigint is inf, data == NULL and len != 0
     * -> len > 0 : +inf, else means -inf
     */
    len_type_ len;
    data_type_* data;
} si_bigint;

si_bigint* si_bigint_new_from_num(intmax_t const);
#ifndef NDEBUG
si_bigint* si_bigint_new_from_multi_num_(len_type_, ...);
#endif
si_bigint* si_bigint_new_from_str(char const*);
si_bigint* si_bigint_new_from_si_bigint(si_bigint const*const);
void si_bigint_del(si_bigint *);
bool si_bigint_is_NaN(si_bigint const*const);
bool si_bigint_is_inf(si_bigint const*const);
void si_bigint_to_bcd(si_bigint **const);
#ifndef SI_BIGINT_NO_PRINT
void si_bigint_print(si_bigint const*const);
#endif
void si_bigint_abs(si_bigint *const);

// TODO: support double, and how to tackle negative numbers
void si_bigint_add(si_bigint **const, si_bigint const*const);
void si_bigint_add_num(si_bigint **const, intmax_t const);
void si_bigint_sub(si_bigint **const, si_bigint const*const);
void si_bigint_sub_num(si_bigint **const, intmax_t const);
void si_bigint_mul(si_bigint **const, si_bigint const*const);
void si_bigint_mul_num(si_bigint **const, intmax_t const);
void si_bigint_div(si_bigint **const, si_bigint const*const, double *);
void si_bigint_div_num(si_bigint **const, intmax_t const, double *);
void si_bigint_mod(si_bigint **const, si_bigint const*const, uintmax_t *);
void si_bigint_mod_num(si_bigint **const, intmax_t const, uintmax_t *);
void si_bigint_pow(si_bigint **const, si_bigint const*const);
void si_bigint_pow_num(si_bigint **const, intmax_t const);

void si_bigint_shl(si_bigint **const);
void si_bigint_shr(si_bigint *const);
void si_bigint_not(si_bigint *const);
void si_bigint_and(si_bigint **const, si_bigint const*const);
void si_bigint_and_num(si_bigint *const, intmax_t const);
void si_bigint_or(si_bigint **const, si_bigint const*const);
void si_bigint_or_num(si_bigint *const, intmax_t const);
void si_bigint_xor(si_bigint **const, si_bigint const*const);
void si_bigint_xor_num(si_bigint *const, intmax_t const);

bool si_bigint_eq(si_bigint const*const, si_bigint const*const);
bool si_bigint_eq_num(si_bigint const*const, intmax_t const);
bool si_bigint_lt(si_bigint const*const, si_bigint const*const);
bool si_bigint_lt_num(si_bigint const*const, intmax_t const);
bool si_bigint_gt(si_bigint const*const, si_bigint const*const);
bool si_bigint_gt_num(si_bigint const*const, intmax_t const);

#ifdef __cplusplus
}
#endif // __cplusplus
