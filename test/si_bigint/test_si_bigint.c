#include "../../sint/si_bigint/si_bigint.h"
#include <assert.h>

#define Def_TEST_(func_name) void test_si_bigint_##func_name(void)
#define Run_TEST_(func_name) test_si_bigint_##func_name()

Def_TEST_(eq) {
    si_bigint *a = si_bigint_new_from_num(1);
    si_bigint *b = si_bigint_new_from_num(1);
    si_bigint *c = si_bigint_new_from_num(-1);

    assert(si_bigint_eq_num(a, 1));
    assert(si_bigint_eq_num(c, -1));
    // assert(si_bigint_eq(a, b));
    // assert(!si_bigint_eq(a, c));
}

int main(void) {
    Run_TEST_(eq);

    return 0;
}
