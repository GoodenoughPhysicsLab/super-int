#pragma once

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "../sint/si_bigint/si_bigint.h"
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <gtest/gtest.h>

GTEST_TEST(eq, _) {
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

GTEST_TEST(and, _) {
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
