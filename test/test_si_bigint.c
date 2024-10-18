#include <stdio.h>
#include <stdlib.h>
#include "../sint/si_bigint/si_bigint.h"

#define Def_TEST(name) void Def_TEST_si_bigint_##name()
#define Run_TEST(name)      Def_TEST_si_bigint_##name()
#define ASSERT_TRUE(expression) \
    if (!(expression)) { \
        printf("assertion failed: %s\n", #expression); \
        abort(); \
    }

Def_TEST(eq) {
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

Def_TEST(not) {
    si_bigint *a = si_bigint_new_from_multi_num_(-4ll, 1ull, 1ull, 1ull, 2ull);
    si_bigint_not(a);
    ASSERT_TRUE(a->len == 4);
    ASSERT_TRUE(a->data[0] == ~(data_type_)1);
    ASSERT_TRUE(a->data[1] == ~(data_type_)1);
    ASSERT_TRUE(a->data[2] == ~(data_type_)1);
    ASSERT_TRUE(a->data[3] == ~(data_type_)2);
    si_bigint_del(a);
}

Def_TEST(and) {
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

int main() {
    Run_TEST(eq);
    Run_TEST(not);
    Run_TEST(and);

    return 0;
}
