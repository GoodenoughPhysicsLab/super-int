#if __STDC_VERSION__ < 199901L
    #error "C99 or later is required"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#ifndef SI_BIGINT_NO_PRINT
    #include <stdio.h>
#endif
#include <si_bigint.h>

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

#if defined(__GNUC__) || defined(__clang__)
    #define SI_noreturn __attribute__((noreturn))
#elif defined(_MSC_VER)
    #define SI_noreturn __declspec(noreturn)
#elif 201112L <= __STDC_VERSION__ && __STDC_VERSION__ <= 201710L
    #define SI_noreturn _Noreturn
#elif __STDC_VERSION >= 202311L
    #define SI_noreturn [[noreturn]]
#else
    #define SI_noreturn
#endif

#define SI_private static inline

#ifndef SI_export
    #define SI_export
#endif

#ifdef SI_BIGINT_NO_PRINT
    #define SI_throw(exception, _) abort()
#else
    #define SI_throw(exception, err_msg) \
        do { \
            perror("si_bigint::" #exception  ": "  err_msg "\n"); \
            abort(); \
        } while(0)
#endif

#define Def_EXCEPTION(exception, err_msg) \
    SI_noreturn SI_private \
    void throw_##exception(void) { \
        SI_throw(exception, err_msg); \
    }

/* si_bigint::BadAllocError
 */
Def_EXCEPTION(BadAllocError, "failed to allocate memory")

/* Get the length of a si_bigint
 */
SI_private
size_t get_si_bigint_len_(si_bigint const*const num) {
    assert(num != NULL);
    return (size_t)(num->len < 0 ? -num->len : num->len);
}

/* Calculate the size of a si_bigint
 */
SI_private
size_t sizeof_si_bigint_(si_bigint const *num) {
    assert(num != NULL);
    return sizeof(si_bigint) + get_si_bigint_len_(num) * sizeof(si_data_type);
}

/* expand memory of si_bigint
 */
SI_private
void expand_memory_(si_bigint **const num, size_t const len) {
    assert(num != NULL && *num != NULL);

    size_t pre_len = get_si_bigint_len_(*num);
    assert(len > pre_len);

    si_bigint *tmp = (si_bigint*)realloc(
        *num, sizeof(si_bigint) + sizeof(si_data_type) * len
    );
    if (tmp == NULL) {
        free(*num);
        throw_BadAllocError();
    }

    tmp->data = (si_data_type*)&(tmp->data) + 1;
    memset(&tmp->data[pre_len], 0, (len - pre_len) * sizeof(si_data_type));
    tmp->len = (*num)->len < 0 ? -len : len;
    *num = tmp;
}

/* Create a new si_bigint from a number
 */
SI_export
si_bigint* new_si_bigint_from_num(intmax_t const num) {
    si_bigint *res = (si_bigint*)malloc(sizeof(si_bigint) + sizeof(si_data_type));
    if (res == NULL) {
        throw_BadAllocError();
    }
    res->len = num < 0 ? -1 : 1;
    res->data = (si_data_type*)(&(res->data) + 1);
    res->data[0] = (si_data_type)(num < 0 ? -num : num);
    return res;
}

/* Create a new si_bigint from a string
 */
// si_bigint* new_si_bigint_from_str(char const* str) {
//     assert(str != NULL);

//     if (strlen(str) == 0) {
//         return new_si_bigint_from_num(0);
//     }

//     bool is_negative;
//     if (str[0] == '-') {
//         is_negative = true;
//         ++str;
//     } else {
//         is_negative = false;
//     }

//     si_bigint *res;
//     if (str[0] == '0' && str[1] == 'b') {
//         res = (si_bigint*)malloc(strlen(str) - 2);
//         // TODO
//     } else if (str[0] == '0' && str[1] == 'o') {
//         //
//     } else if (str[0] == '0' && str[1] == 'x') {
//         //
//     } else {
//         //
//     }

//     return res;
// }

/* Create a new si_bigint from a si_bigint
 */
SI_export
si_bigint* new_si_bigint_from_si_bigint(si_bigint const*const num) {
    assert(num != NULL);

    si_bigint *res = (si_bigint*)malloc(sizeof_si_bigint_(num));
    if (res == NULL) {
        throw_BadAllocError();
    }
    res->data = (si_data_type*)(&(res->data) + 1);
    memcpy(res, num, sizeof_si_bigint_(num));
    return res;
}

/* Free memory of si_bigint
 */
SI_export
void del_si_bigint(si_bigint *num) {
    assert(num != NULL);

    free(num);
}

/* Return true if a si_bigint is NaN
 */
SI_export
bool si_bigint_is_NaN(si_bigint const*const num) {
    return num->len == 0;
}

/* Return true if a si_bigint is infinity
 */
SI_export
bool si_bigint_is_inf(si_bigint const*const num) {
    return num->data == NULL && num->len != 0;
}

/* Return true if a si_bigint is NaN or infinity
 */
SI_private
bool si_bigint_is_NaN_or_inf(si_bigint const*const num) {
    return si_bigint_is_NaN(num) || si_bigint_is_inf(num);
}

/* if num < 0, then two's complement
 */
SI_private
void twos_complement_(si_bigint *const num) {
    assert(num != NULL && num->len < 0 && !si_bigint_is_inf(num));

    size_t num_len = get_si_bigint_len_(num);
#ifdef SINT_SIMD // TODO: bugfix: write overflow
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
    // si_bigint_and_num(num, 1); // TODO
    num->len = -num->len;
}

SI_export
void si_bigint_to_bcd(si_bigint **const num) {
    assert(num != NULL && *num != NULL);

    // TODO
}

#ifndef SI_BIGINT_NO_PRINT
/* Print a si_bigint to stdout
 */
// void si_bigint_print(si_bigint const*const num) {
//     assert(num != NULL);

//     si_bigint *tmp = new_si_bigint_from_si_bigint(num);

//     // TODO
// }
#endif // SI_BIGINT_NO_PRINT

/* absolute value of itself
 */
SI_export
void si_bigint_abs(si_bigint *const num) {
    assert(num != NULL);

    num->len = (si_len_type)get_si_bigint_len_(num);
}

/* num1 += num2
 */
SI_private
void si_bigint_add_num_(si_bigint **const num1, si_data_type const num2) {
    assert(num1 != NULL && *num1 != NULL);

    bool is_overflow;
    if ((*num1)->data[0] > UINTMAX_MAX - num2) {
        is_overflow = true;
    } else {
        is_overflow = false;
    }

    (*num1)->data[0] += num2;

    for (size_t i = 1; i < get_si_bigint_len_(*num1); ++i) {
        bool is_overflow_ = is_overflow;

        if ((*num1)->data[i] > UINTMAX_MAX - 1) {
            is_overflow = true;
        } else {
            is_overflow = false;
        }

        if (is_overflow_) {
            ++(*num1)->data[i];
        }
    }

    if (is_overflow) {
        expand_memory_(num1, (*num1)->len + 1);
        (*num1)->data[(*num1)-> len - 1] = 1;
    }
}

/* num1 -= num2
 */
SI_private
void si_bigint_sub_num_(si_bigint **const num1, si_data_type const num2) {

}

/* Add a number to a si_bigint
 */
SI_export
void si_bigint_add_num(si_bigint **const num1, intmax_t const num2) {
    assert(num1 != NULL && *num1 != NULL);

    if (si_bigint_is_NaN_or_inf(*num1)) {
        return;
    }

    if (num2 < 0) {
        si_bigint_sub_num_(num1, (si_data_type)(-num2));
    } else {
        si_bigint_add_num_(num1, (si_data_type)num2);
    }
}

SI_export
void si_bigint_sub_num(si_bigint **const num1, intmax_t const num2) {
    assert(num1 != NULL && *num1 != NULL);

    if (si_bigint_is_NaN_or_inf(*num1)) {
        return;
    }

    if (num2 < 0) {
        si_bigint_add_num_(num1, (si_data_type)(-num2));
    } else {
        si_bigint_sub_num_(num1, (si_data_type)num2);
    }
}

/* Reverse si_bigint
 *
 *  @prama num: The si_bigint number to be reversed
 *
 * NOTE: this just for the same behavior as reversing a signed type(etc. int)
 */
// void si_bigint_not(si_bigint *const num) {
//     assert(num != NULL);

//     if (si_bigint_is_NaN_or_inf(num)) {
//         return;
//     }

//     // TODO
// }

/* Bitwise AND with a number
 */
SI_export
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
SI_export
void si_bigint_and(si_bigint **const num1, si_bigint const*const num2) { // TODO: -128 & -127
    assert(num1 != NULL && *num1 != NULL && num2 != NULL);

    if (si_bigint_is_NaN_or_inf(*num1) || si_bigint_is_NaN_or_inf(num2)) {
        return;
    }

    size_t num2_len = get_si_bigint_len_(num2);
    if (get_si_bigint_len_(*num1) < num2_len) {
        expand_memory_(num1, num2_len);
    }
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

SI_private
bool si_bigint_eq_(si_bigint const*const restrict num1, si_bigint const*const restrict num2) {
    assert(num1 != NULL && num2 != NULL && num1 != num2);

    if (num1->len > 0 && num2->len < 0 || num1->len < 0 && num2->len > 0) {
        return false;
    }

    size_t num1_len = get_si_bigint_len_(num1);
    size_t num2_len = get_si_bigint_len_(num2);
    assert(num1_len <= num2_len);

    if (memcmp(num1->data, num2->data, num1_len * sizeof(si_data_type)) != 0) {
        return false;
    }
    for (size_t i = num1_len; i < num2_len; ++i) {
        if (num2->data[i] != 0) {
            return false;
        }
    }
    return true;
}

/* Compare two si_bigint
 */
SI_export
bool si_bigint_eq(si_bigint const*const restrict num1, si_bigint const*const restrict num2) {
    assert(num1 != NULL && num2 != NULL && num1 != num2);

    if (si_bigint_is_NaN_or_inf(num1) || si_bigint_is_NaN_or_inf(num2)) {
        return false;
    }

    if (get_si_bigint_len_(num1) <= get_si_bigint_len_(num2)) {
        return si_bigint_eq_(num1, num2);
    } else {
        return si_bigint_eq_(num2, num1);
    }
}

/* Compare a si_bigint with a number
 */
SI_export
bool si_bigint_eq_num(si_bigint const*const num1, intmax_t const num2) {
    assert(num1 != NULL);

    if (si_bigint_is_NaN_or_inf(num1)
        || num2 < 0 && num1->len > 0 || num2 >= 0 && num1->len < 0
        || num1->data[0] != (si_data_type)(num2 < 0 ? -num2 : num2))
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
