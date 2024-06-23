#pragma once

#if defined(_WIN32) || defined(_WIN64)
#   define BIGINT_IS_WINDOWS
#endif

#ifdef __cplusplus

#else
#   ifdef __STDC_VERSION__
#       if __STDC_VERSION__ >= 199901L
#           define BIGINT_IS_C99
#       endif
#       if __STDC_VERSION__ >= 201112L
#           define BIGINT_IS_C11
#       endif
#       if __STDC_VERSION__ >= 201710L
#           define BIGINT_IS_C17
#       endif
#       if __STDC_VERSION__ >= 202311L
#           define BIGINT_IS_C23
#       endif
#   endif
#endif