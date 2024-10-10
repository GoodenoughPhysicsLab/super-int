#pragma once

#include "../sint/si_bigint/si_bigint.h"
#include <stdlib.h>
#include <gtest/gtest.h>

GTEST_TEST(eq, _) {
    si_bigint *a = si_bigint_new_from_num(1);
    si_bigint *b = si_bigint_new_from_num(1);
    si_bigint *c = si_bigint_new_from_num(-1);

    ASSERT_TRUE(si_bigint_eq_num(a, 1));
    ASSERT_TRUE(si_bigint_eq_num(c, -1));
    // assert(si_bigint_eq(a, b));
    // assert(!si_bigint_eq(a, c));

    si_bigint_del(a);
    si_bigint_del(b);
    si_bigint_del(c);
}

GTEST_TEST(not, _) {
    si_bigint *a = si_bigint_new_from_multi_num_(-2, 1, 2);
    si_bigint_not(a);

    ASSERT_TRUE(a->len == 2);
    ASSERT_TRUE(a->data[0] == ~(data_type_)1);
    ASSERT_TRUE(a->data[1] == ~(data_type_)2);
}

GTEST_TEST(and, _) {
    si_bigint *a = si_bigint_new_from_num(3);
    si_bigint *b = si_bigint_new_from_multi_num_(-2, 1, 5);
    ASSERT_TRUE(b->len == -2);
    ASSERT_TRUE(b->data[0] == 1);
    ASSERT_TRUE(b->data[1] == 5);

    si_bigint_and(&a, b);
    ASSERT_TRUE(si_bigint_eq_num(a, 1));

    si_bigint_del(a);
    si_bigint_del(b);
}
