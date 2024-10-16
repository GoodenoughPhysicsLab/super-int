#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "si_bigint.h"
#ifndef SI_BIGINT_NO_PRINT
#include <stdio.h>
#endif

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
 * Get the length of a si_bigint
 */
inline static size_t get_si_bigint_len_(si_bigint const*const num) {
    assert(num != NULL);
    return (size_t)(num->len < 0 ? -num->len : num->len);
}

/* [[ Private ]]
 * Calculate the size of a si_bigint
 */
static size_t sizeof_si_bigint_(si_bigint const *num) {
    assert(num != NULL);
    return sizeof(si_bigint) + get_si_bigint_len_(num) * sizeof(data_type_);
}

/* [[ Private ]]
 * Reallocate memory of a si_bigint
 */
static void realloc_si_bigint_(si_bigint **const num, size_t const len) {
    assert(num != NULL && *num != NULL);

    size_t pre_len = get_si_bigint_len_(*num);
    if (len > pre_len) {
        si_bigint *tmp = (si_bigint*)realloc(
            *num, sizeof(si_bigint) + sizeof(data_type_) * len
        );
        if (tmp == NULL) {
            free(*num);
            *num = NULL;
#ifndef SI_BIGINT_NO_PRINT
            perror("si_bigint::BadAllocError: fail to reallocate memory");
#endif
            abort();
        }

        if ((*num)->len < 0) {
            tmp->len = -len;
        } else {
            tmp->len = len;
        }
        tmp->data = (data_type_*)(&(tmp->data) + 1);
        memset(&tmp->data[pre_len], 0, (len - pre_len) * sizeof(data_type_));
        *num = tmp;
    }
}

/* Create a new si_bigint from a number
 */
si_bigint* si_bigint_new_from_num(intmax_t const num) {
    si_bigint *res = (si_bigint*)malloc(sizeof(si_bigint) + sizeof(data_type_));
if (res == NULL) {
#ifndef SI_BIGINT_NO_PRINT
        perror("si_bigint::BadAllocError: fail to allocate memory");
#endif
        abort();
    }
    res->len = num < 0 ? -1 : 1;
    res->data = (data_type_*)(&(res->data) + 1);
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
        + (sign_and_len_arg < 0 ? -sign_and_len_arg : sign_and_len_arg) * sizeof(data_type_)
    );
    if (res == NULL) {
#ifndef SI_BIGINT_NO_PRINT
        perror("si_bigint::BadAllocError: fail to allocate memory\n");
#endif
        abort();
    }
    res->data = (data_type_*)(&(res->data) + 1);
    res->len = sign_and_len_arg;
    for (len_type_ i = 0; i != (sign_and_len_arg < 0 ? -sign_and_len_arg : sign_and_len_arg); ++i) {
        res->data[i] = va_arg(args, data_type_);
    }
    va_end(args);

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
    } else if (str[0] == '0' && str[1] == 'o') {
        //
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
if (res == NULL) {
#ifndef SI_BIGINT_NO_PRINT
        perror("si_bigint::BadAllocError: fail to allocate memory");
#endif
        abort();
    }
    res->data = (data_type_*)(&(res->data) + 1);
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

/* Return true if a si_bigint is infinity
 */
bool si_bigint_is_inf(si_bigint const*const num) {
    return num->data == NULL && num->len != 0;
}

/* [[ Private ]]
 * Return true if a si_bigint is NaN or infinity
 */
inline static bool si_bigint_is_NaN_or_inf(si_bigint const*const num) {
    return si_bigint_is_NaN(num) || si_bigint_is_inf(num);
}

void si_bigint_to_bcd(si_bigint **const num) {
    assert(num != NULL && *num != NULL);

    // TODO
}

#ifndef SI_BIGINT_NO_PRINT
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

    num->len = (len_type_)get_si_bigint_len_(num);
}

/* Reverse every bit of itself
 *
 *  @prama num: The si_bigint number to be reversed
 */
void si_bigint_not(si_bigint *const num) {
    assert(num != NULL);

    if (si_bigint_is_NaN_or_inf(num)) {
        return;
    }

    size_t num_len = get_si_bigint_len_(num);
#ifdef SINT_SIMD
    #if defined(__AVX2__)
    for (size_t i = 0; i < num_len;
        #if defined(UINTMAX_T_IS_64BIT)
                i += 4
        #elif defined(UINTMAX_T_IS_32BIT)
                i += 8
        #else
            #error "your old mechine support simd?"
        #endif
    ) {
        __m256i tmp = _mm256_loadu_si256((__m256i*)&num->data[i]);
        tmp = _mm256_xor_si256(tmp, _mm256_set1_epi64x(-1));
        _mm256_storeu_si256((__m256i*)&num->data[0], tmp);
    }
    #elif defined(__ARM_NEON__)
        #if defined(UINTMAX_T_IS_64BIT)
    for (size_t i = 0; i < num_len; i += 2) {
        uint64x2_t tmp = vld1q_u64(&num->data[i]);
        tmp = veorq_u64(tmp, vdupq_n_u64(-1));
        vst1q_u64(&num->data[i], tmp);
    }
        #elif defined(UINTMAX_T_IS_32BIT) // ^^^ UINTMAX_T_IS_64BIT / vvv UINTMAX_T_IS_32BIT
    for (size_t i = 0; i < num_len; i += 4) {
        uint8x32_t tmp = vld1q_u32(&num->data[i]);
        tmp = veorq_u32(tmp, vdupq_n_u32(-1));
        vst1q_u32(&num->data[i], tmp);
    }
        #endif // UINTMAX_T_IS_32BIT
    #endif // __ARM_NEON__
#else // ^^^ SINT_SIMD / vvv !SINT_SIMD
    for (size_t i = 0; i < num_len; ++i) {
        num->data[i] = ~num->data[i];
    }
#endif // !SINT_SIMD
    num->len = -num->len;
}

/* Bitwise AND with a number
 */
void si_bigint_and_num(si_bigint *const num1, intmax_t const num2) {
    assert(num1 != NULL);

    if (si_bigint_is_NaN_or_inf(num1)) {
        return;
    }

    if (!(num1->len < 0 && num2 < 0)) {
        num1->len = num1->len < 0 ? -num1->len : num1->len;
    }
    num1->data[0] &= num2;
}

/* Bitwise AND with another si_bigint
 */
void si_bigint_and(si_bigint **const num1, si_bigint const*const num2) {
    assert(num1 != NULL && *num1 != NULL && num2 != NULL);

    if (si_bigint_is_NaN_or_inf(*num1) || si_bigint_is_NaN_or_inf(num2)) {
        return;
    }

    size_t num2_len = get_si_bigint_len_(num2);
    realloc_si_bigint_(num1, num2_len);
    if (!((*num1)->len < 0 && num2->len < 0)) {
        (*num1)->len = (*num1)->len < 0 ? -(*num1)->len : (*num1)->len;
    }
#ifdef SINT_SIMD
    #if defined(__AVX2__)
    for (size_t i = 0; i < num2_len;
        #if defined(UINTMAX_T_IS_64BIT)
                i += 4
        #elif defined(UINTMAX_T_IS_32BIT)
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
    #elif defined(__ARM_NEON__) // ^^^ __AVX2__ / vvv __ARM_NEON__
        #if defined(UINTMAX_T_IS_64BIT)
    for (size_t i = 0; i < num2_len; i += 2) {
        uint64x2_t tmp1 = vld1q_u64(&(*num1)->data[i]);
        uint64x2_t tmp2 = vld1q_u64(&num2->data[i]);
        tmp1 = vandq_u64(tmp1, tmp2);
        vst1q_u64(&(*num1)->data[i], tmp1);
    }
        #elif defined(UINTMAX_T_IS_32BIT) // ^^^ UINTMAX_T_IS_64BIT / vvv UINTMAX_T_IS_32BIT
    for (size_t i = 0; i < num2_len; i += 4) {
        uint32x4_t tmp1 = vld1q_u32(&(*num1)->data[i]);
        uint32x4_t tmp2 = vld1q_u32(&num2->data[i]);
        tmp1 = vandq_u32(tmp1, tmp2);
        vst1q_u32(&(*num1)->data[i], tmp1);
    }
        #endif // UINTMAX_T_IS_32BIT
    #else // ^^^ __ARM_NEON__ / vvv !__ARM_NEON__
        #error "simd (avx2 or neon) not support"
    #endif // __ARM_NEON__
#else // ^^^ SINT_SIMD / vvv !SINT_SIMD
    for (size_t i = 0; i < num2_len; ++i) {
        (*num1)->data[i] &= num2->data[i];
    }
#endif // !SINT_SIMD
}

/* Compare a si_bigint with a number
 */
bool si_bigint_eq_num(si_bigint const*const num1, intmax_t const num2) {
    assert(num1 != NULL);

    if (si_bigint_is_NaN_or_inf(num1)
        || num2 < 0 && num1->len > 0 || num2 >= 0 && num1->len < 0
        || num1->data[0] != (data_type_)(num2 < 0 ? -num2 : num2))
    {
        return false;
    }
    size_t num1_len = get_si_bigint_len_(num1);
    for (size_t i = 1; i < num1_len; ++i) {
        if (num1->data[i] != 0) {
            return false;
        }
    }
    return true;
}
