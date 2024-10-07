#pragma once

#if defined(_WIN32) || defined(_WIN64)
#   define BIGINT_IS_WINDOWS
#endif

#if defined(_MSC_VER) && __cplusplus == 199711L
#    if _MSVC_LANG >= 201402L
#        define IS_CPP14
#    endif
#    if _MSVC_LANG > 201402L
#        define IS_CPP17
#    endif
#    if _MSVC_LANG >= 202002L
#        define IS_CPP20
#    endif
#elif !(defined(_MSC_VER) && __cplusplus == 199711L)
#    if __cplusplus >= 201402L
#        define IS_CPP14
#    endif
#    if __cplusplus >= 201703L
#        define IS_CPP17
#    endif
#    if __cplusplus >= 202002L
#        define IS_CPP20
#    endif
#endif