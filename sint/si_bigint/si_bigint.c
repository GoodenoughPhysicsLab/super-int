#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "si_bigint.h"

#ifdef SI_BIGINT_SIMD
    #include <intrin.h>
#endif

typedef intmax_t len_type_;
typedef uintmax_t data_type_;

struct si_bigint {
    /* if si_bigint is negative, len < 0, else len > 0
     *
     * if si_bigint is NaN, len == 0
     */
    len_type_ len;
    data_type_ data[1];
};

/* [[ Private ]]
 * Calculate the size of a si_bigint
 */
static size_t sizeof_si_bigint_(si_bigint const *num) {
    assert(num != NULL);
    return sizeof(si_bigint) + (num->len - 1) * sizeof(uintmax_t);
}

/* [[ Private ]]
 * Get the length of a si_bigint
 */
static len_type_ get_si_bigint_len_(si_bigint const*const num) {
    assert(num != NULL);

    return num->len < 0 ? -num->len : num->len;
}

/* [[ Private ]]
 * Reallocate memory of a si_bigint
 */
static void realloc_si_bigint_(si_bigint **const num, size_t const len) {
    assert(num != NULL && *num != NULL);

    len_type_ pre_len = get_si_bigint_len_(*num);
    if (len > pre_len) {
        *num = (si_bigint*)realloc(
            *num, sizeof(si_bigint) + sizeof(data_type_) * (len - 1)
        );
        memset(&(*num)->data[pre_len - 1], 0, (len - pre_len) * sizeof(data_type_));
    }
    assert(*num != NULL);
}

/* Create a new si_bigint from a number
 */
si_bigint* si_bigint_new_from_num(intmax_t const num) {
    si_bigint *res = (si_bigint*)malloc(sizeof(si_bigint));
    res->len = num < 0 ? -1 : 1;
    res->data[0] = (data_type_)(num < 0 ? -num : num);
    return res;
}

/* Create a new si_bigint from a string
 */
si_bigint* si_bigint_new_from_str(char const*const str) {
    assert(str != NULL);

    si_bigint *res;
    if (str[0] == '0' && str[1] == 'b') {
        // TODO
    } else if (str[0] == '0' && str[1] == 'x') {
        //
    } else {
        //
    }

    return res;
}

/* Create a new si_bigint from a si_bigint
 */
si_bigint* si_bigint_new_from_si_bigint(si_bigint const*const num) {
    assert(num != NULL);

    si_bigint *res;
    memcpy(res, num, sizeof_si_bigint_(num));
    return res;
}

/* Free memory of si_bigint
 */
void si_bigint_del(si_bigint *num) {
    assert(num != NULL);

    free(num);
}

/* Return true if a si_bigint is NaN
 */
bool si_bigint_is_NaN(si_bigint const*const num) {
    return num->len == 0;
}

void si_bigint_to_bcd(si_bigint **const num) {
    assert(num != NULL && *num != NULL);

    // TODO
}

#ifndef SI_BIGINT_NO_PRINT
#include <stdio.h>

/* Print a si_bigint to stdout
 */
void si_bigint_print(si_bigint const*const num) {
    assert(num != NULL);

    si_bigint *tmp = si_bigint_new_from_si_bigint(num);

    // TODO
}
#endif

/* absolute value of itself
 */
void si_bigint_abs(si_bigint *const num) {
    assert(num != NULL);

    num->len = get_si_bigint_len_(num);
}

/* Reverse every bit of itself
 *
 *  @prama num: The si_bigint number to be reversed
 */
void si_bigint_not(si_bigint *const num) {
    assert(num != NULL);

    if (si_bigint_is_NaN(num)) {
        return;
    }

    for (int i = 0; i < get_si_bigint_len_(num); ++i) {
        num->data[i] = ~num->data[i];
    }
    return;
}

/* Bitwise AND with a number
 */
void si_bigint_and_num(si_bigint *const num1, uintmax_t const num2) {
    assert(num1 != NULL);

    if (si_bigint_is_NaN(num1)) {
        return;
    }

    num1->data[0] &= num2;
}

/* Bitwise AND with another si_bigint
 */
void si_bigint_and(si_bigint **const num1, si_bigint const*const num2) {
    assert(num1 != NULL && *num1 != NULL && num2 != NULL);

    if (si_bigint_is_NaN(*num1) || si_bigint_is_NaN(num2)) {
        return;
    }

    realloc_si_bigint_(num1, get_si_bigint_len_(num2));
#ifdef SI_BIGINT_SIMD
    #if defined(__AVX2__)
    for (int i = 0; i < get_si_bigint_len_(num2);
        #if UINTMAX_MAX == 18446744073709551615ULL
                i += 4
        #elif UINTMAX_MAX == 4294967295UL
                i += 8
        #else
            #error "Your old mechine support simd?"
        #endif // UINTMAX_MAX
    ) {
        __m256i tmp1 = _mm256_loadu_si256((__m256i*)&(*num1)->data[i]);
        __m256i tmp2 = _mm256_loadu_si256((__m256i*)&num2->data[i]);
        tmp1 = _mm256_and_si256(tmp1, tmp2);
        _mm256_storeu_si256((__m256i*)&(*num1)->data[i], tmp1);
    }
    #elif defined(__ARM_NEON__)
        //
    #endif // __AVX2__
#else // ^^^ SI_BIGINT_SIMD / vvv !SI_BIGINT_SIMD
    for (int i = 0; i < get_si_bigint_len_(num2); ++i) {
        (*num1)->data[i] &= num2->data[i];
    }
#endif // SI_BIGINT_SIMD
}

/* Compare a si_bigint with a number
 */
bool si_bigint_eq_num(si_bigint const*const num1, intmax_t const num2) {
    assert(num1 != NULL);

    if (get_si_bigint_len_(num1) != 1 || si_bigint_is_NaN(num1)) {
        return false;
    }
    if (num1->len < 0 && num2 < 0 || num1->len > 0 && num2 >= 0) {
        return num1->data[0] == (data_type_)(num2 < 0 ? -num2 : num2);
    }
    return false;
}
