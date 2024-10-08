#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "si_bigint.h"

#ifdef SINT_SIMD
    #if defined(__AVX2__)
        #ifdef _WIN32
            #include <intrin.h>
        #else // ^^^ _WIN32 / vvv POSIX
            #include <immintrin.h>
        #endif // _WIN32
    #elif defined(__ARM_NEON__)
        #include <arm_neon.h>
    #else
        #error "simd (avx2 or neon) not support"
    #endif // __AVX2__
#endif // SINT_SIMD

#if UINTMAX_MAX == 18446744073709551615ULL // 64 bit
    #define UINTMAX_T_IS_64BIT
#elif UINTMAX_MAX == 4294967295UL // 32 bit
    #define UINTMAX_T_IS_32BIT
#endif

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
static int realloc_si_bigint_(si_bigint **const num, size_t const len) {
    assert(num != NULL && *num != NULL);

    len_type_ pre_len = get_si_bigint_len_(*num);
    if (len > pre_len) {
        si_bigint *tmp = (si_bigint*)realloc(
            *num, sizeof(si_bigint) + sizeof(data_type_) * (len - 1)
        );
        if (tmp == NULL) {
            free(*num);
            *num = NULL;
            return 1;
        }

        if ((*num)->len < 0) {
            tmp->len = -len;
        } else {
            tmp->len = len;
        }
        memset(&tmp->data[pre_len], 0, (len - pre_len) * sizeof(data_type_));
        *num = tmp;
    }
    return 0;
}

/* Create a new si_bigint from a number
 */
si_bigint* si_bigint_new_from_num(intmax_t const num) {
    si_bigint *res = (si_bigint*)malloc(sizeof(si_bigint));
    res->len = num < 0 ? -1 : 1;
    res->data[0] = (data_type_)(num < 0 ? -num : num);
    return res;
}

#ifndef NDEBUG
#include <stdarg.h>
/* Create a new si_bigint from a multi-number
 */
si_bigint* si_bigint_new_from_multi_num_(len_type_ sign_and_len_arg, ...) {
    va_list args;
    va_start(args, sign_and_len_arg);

    si_bigint *res = (si_bigint*)malloc(
        sizeof(si_bigint)
        + ((sign_and_len_arg < 0 ? -sign_and_len_arg : sign_and_len_arg) - 1) * sizeof(data_type_)
    );
    res->len = sign_and_len_arg;
    for (len_type_ i = 0; i != (sign_and_len_arg < 0 ? -sign_and_len_arg : sign_and_len_arg); ++i) {
        res->data[i] = va_arg(args, data_type_);
    }

    return res;
}
#endif // NDEBUG

/* Create a new si_bigint from a string
 */
si_bigint* si_bigint_new_from_str(char const* str) {
    assert(str != NULL);

    if (strlen(str) == 0) {
        return si_bigint_new_from_num(0);
    }

    bool is_negative;
    if (str[0] == '-') {
        is_negative = true;
        ++str;
    } else {
        is_negative = false;
    }

    si_bigint *res;
    if (str[0] == '0' && str[1] == 'b') {
        res = (si_bigint*)malloc(strlen(str) - 2);
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

    si_bigint *res = (si_bigint*)malloc(sizeof_si_bigint_(num));
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
#endif // SI_BIGINT_NO_PRINT

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

    if (realloc_si_bigint_(num1, get_si_bigint_len_(num2))) {
        abort();
    }
    if ((*num1)->len < 0 && num2->len > 0 || (*num1)->len > 0 && num2->len < 0) {
        (*num1)->len = (*num1)->len > 0 ? -(*num1)->len : (*num1)->len;
    }
#ifdef SINT_SIMD
    for (int i = 0; i < get_si_bigint_len_(num2);
    #if defined(UINTMAX_T_IS_64BIT)
                i += 4
    #elif defined(UINTMAX_T_IS_32BIT)
                i += 8
    #else
            #error "Your old mechine support simd?"
    #endif // UINTMAX_MAX
    ) {
    #if defined(__AVX2__)
        __m256i tmp1 = _mm256_loadu_si256((__m256i*)&(*num1)->data[i]);
        __m256i tmp2 = _mm256_loadu_si256((__m256i*)&num2->data[i]);
        tmp1 = _mm256_and_si256(tmp1, tmp2);
        _mm256_storeu_si256((__m256i*)&(*num1)->data[i], tmp1);
    #elif defined(__ARM_NEON__)
        #if defined(UINTMAX_T_IS_64BIT)
        uint8x16_t tmp1 = vld1q_u8(&(*num1)->data[i]);
        uint8x16_t tmp2 = vld1q_u8(&(*num1)->data[i]);
        tmp1 = vandq_u8(tmp1, tmp2);
        vst1q_u8(&(*num1)->data[i], tmp1);
        #elif defined(UINTMAX_T_IS_32BIT)
        uint8x16_t tmp1 = vld1q_u32(&(*num1)->data[i]);
        uint8x16_t tmp2 = vld1q_u32(&(*num1)->data[i]);
        tmp1 = vandq_u32(tmp1, tmp2);
        vst1q_u32(&(*num1)->data[i], tmp1);
        #endif
    #else
        #error "simd (avx2 or neon) not support"
    #endif // __AVX2__
    }
#else // ^^^ SINT_SIMD / vvv !SINT_SIMD
    for (int i = 0; i < get_si_bigint_len_(num2); ++i) {
        (*num1)->data[i] &= num2->data[i];
    }
#endif // SINT_SIMD
}

/* Compare a si_bigint with a number
 */
bool si_bigint_eq_num(si_bigint const*const num1, intmax_t const num2) {
    assert(num1 != NULL);

    if (si_bigint_is_NaN(num1)
        || num2 < 0 && num1->len > 0 || num2 >= 0 && num1->len < 0
        || num1->data[0] != (num2 < 0 ? -num2 : num2))
    {
        return false;
    }
    len_type_ num1_len = get_si_bigint_len_(num1);
    for (int i = 1; i < num1_len; ++i) {
        if (num1->data[i] != 0) {
            return false;
        }
    }
    return true;
}
