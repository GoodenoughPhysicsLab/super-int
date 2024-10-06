#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../../sint/si_bigint/si_bigint.h"
#include <signal.h>
#include <stdlib.h>
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

    si_bigint_del(a);
    si_bigint_del(b);
    si_bigint_del(c);
}

Def_TEST_(and) {
    si_bigint *a = si_bigint_new_from_num(3);
    si_bigint *b = si_bigint_new_from_multi_num_(-2, 1, 5);
    assert(b->len == -2);
    assert(b->data[0] == 1);
    assert(b->data[1] == 5);

    si_bigint_and(&a, b);
    assert(si_bigint_eq_num(a, -1));

    si_bigint_del(a);
    si_bigint_del(b);
}

void test_exit(int signal) {
    if (signal == SIGINT) {
        exit(1);
    }
}

int main(void) {
    signal(SIGINT, test_exit);
    Run_TEST_(eq);
    Run_TEST_(and);

    return 0;
}
