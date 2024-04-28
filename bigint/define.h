#pragma once

#if defined(_WIN32) || defined(_WIN64)
#   define BIGINT_IS_WINDOWS 1
#else
#   define BIGINT_IS_WINDOWS 0
#endif

#ifdef __STDC_VERSION__
#   if __STDC_VERSION__ >= 199901L
#       define BIGINT_IS_C99 1
#   else
#       define BIGINT_IS_C99 0
#   endif
#   if __STDC_VERSION__ >= 201112L
#       define BIGINT_IS_C11 1
#   else
#       define BIGINT_IS_C11 0
#   endif
#   if __STDC_VERSION__ >= 201710L
#       define BIGINT_IS_C17 1
#   else
#       define BIGINT_IS_C17 0
#   endif
#   if __STDC_VERSION__ >= 202311L
#       define BIGINT_IS_C23 1
#   else
#       define BIGINT_IS_C23 0
#   endif
#else
#   define BIGINT_IS_C99 0
#   define BIGINT_IS_C11 0
#   define BIGINT_IS_C17 0
#   define BIGINT_IS_C23 0
#endif


#include <assert.h>
#ifdef _MSC_VER
#   if BIGINT_IS_C11
#       define BIGINT_STATIC_ASSERT _Static_assert
#   else
#       pragma warning(push)
#       pragma warning(disable: 4003)
#       define BIGINT_STATIC_ASSERT(expr, msg) assert(expr)
#   endif
#else
#   define BIGINT_STATIC_ASSERT _Static_assert
#endif