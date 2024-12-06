#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// This macro is API
#ifndef SI_export
    #define SI_export
#endif

typedef intmax_t si_len_type;
typedef uintmax_t si_data_type;

typedef struct si_bigint {
    /* if si_bigint is negative, len < 0, else len > 0
     *
     * if (len == 0) => si_bigint is NaN
     *
     * if (data == NULL and len != 0) => si_bigint is inf
     * -> len > 0 : +inf, else means -inf
     */
    si_len_type len;
    si_data_type* data;
} si_bigint;

SI_export si_bigint* new_si_bigint_from_num(intmax_t const);
SI_export si_bigint* new_si_bigint_from_str(char const*);
SI_export si_bigint* new_si_bigint_from_si_bigint(si_bigint const*const);
SI_export void del_si_bigint(si_bigint *);
SI_export bool si_bigint_is_NaN(si_bigint const*const);
SI_export bool si_bigint_is_inf(si_bigint const*const);
SI_export void si_bigint_to_bcd(si_bigint **const);
#ifndef SI_BIGINT_NO_PRINT
SI_export void si_bigint_print(si_bigint const*const);
#endif
SI_export void si_bigint_abs(si_bigint *const);

// TODO: support double, and how to tackle negative numbers
SI_export void si_bigint_add(si_bigint **const, si_bigint const*const);
SI_export void si_bigint_add_num(si_bigint **const, intmax_t const);
SI_export void si_bigint_sub(si_bigint **const, si_bigint const*const);
SI_export void si_bigint_sub_num(si_bigint **const, intmax_t const);
SI_export void si_bigint_mul(si_bigint **const, si_bigint const*const);
SI_export void si_bigint_mul_num(si_bigint **const, intmax_t const);
SI_export void si_bigint_div(si_bigint **const, si_bigint const*const, double *);
SI_export void si_bigint_div_num(si_bigint **const, intmax_t const, double *);
SI_export void si_bigint_mod(si_bigint **const, si_bigint const*const, uintmax_t *);
SI_export void si_bigint_mod_num(si_bigint **const, intmax_t const, uintmax_t *);
SI_export void si_bigint_pow(si_bigint **const, si_bigint const*const);
SI_export void si_bigint_pow_num(si_bigint **const, intmax_t const);

SI_export void si_bigint_shl(si_bigint **const);
SI_export void si_bigint_shr(si_bigint *const);
SI_export void si_bigint_not(si_bigint *const);
SI_export void si_bigint_and(si_bigint **const, si_bigint const*const);
SI_export void si_bigint_and_num(si_bigint *const, intmax_t const);
SI_export void si_bigint_or(si_bigint **const, si_bigint const*const);
SI_export void si_bigint_or_num(si_bigint *const, intmax_t const);
SI_export void si_bigint_xor(si_bigint **const, si_bigint const*const);
SI_export void si_bigint_xor_num(si_bigint *const, intmax_t const);
SI_export void si_bigint_twos_complement(si_bigint **const);

SI_export bool si_bigint_eq(si_bigint const*const restrict, si_bigint const*const restrict);
SI_export bool si_bigint_eq_num(si_bigint const*const, intmax_t const);
SI_export bool si_bigint_lt(si_bigint const*const, si_bigint const*const);
SI_export bool si_bigint_lt_num(si_bigint const*const, intmax_t const);
SI_export bool si_bigint_gt(si_bigint const*const, si_bigint const*const);
SI_export bool si_bigint_gt_num(si_bigint const*const, intmax_t const);

#ifdef __cplusplus
}
#endif // __cplusplus
