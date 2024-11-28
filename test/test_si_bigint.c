#ifdef NDEBUG
    #undef NDEBUG
#endif

#include <assert.h>
#include <si_bigint.h>

#define Def_TEST(name) void Def_TEST_si_bigint_##name()
#define Run_TEST(name)      Def_TEST_si_bigint_##name()

Def_TEST(eq) {
    si_bigint *a = new_si_bigint_from_num(1);
    si_bigint *b = new_si_bigint_from_num(1);
    si_bigint *c = new_si_bigint_from_num(-1);

    assert(si_bigint_eq_num(a, 1));
    assert(!si_bigint_eq_num(a, 2));
    assert(si_bigint_eq_num(c, -1));
    assert(!si_bigint_eq_num(c, -2));
    // assert(si_bigint_eq(a, b));
    // assert(!si_bigint_eq(a, c));

    del_si_bigint(a);
    del_si_bigint(b);
}

// Def_TEST(not) {
//     si_bigint *a = new_si_bigint_from_multi_num_(-4ll, 1ull, 1ull, 1ull, 2ull);
//     si_bigint_not(a);
//     assert(a->len == 4);
//     assert(a->data[0] == ~(data_type_)1);
//     assert(a->data[1] == ~(data_type_)1);
//     assert(a->data[2] == ~(data_type_)1);
//     assert(a->data[3] == ~(data_type_)2);
//     si_bigint_del(a);
// }

// Def_TEST(and) {
//     si_bigint *a = new_si_bigint_from_num(3);
//     si_bigint *b = new_si_bigint_from_multi_num_(-2, 1, 5);
//     assert(b->len == -2);
//     assert(b->data[0] == 1);
//     assert(b->data[1] == 5);

//     si_bigint_and(&a, b);
//     assert(si_bigint_eq_num(a, 1));

//     del_si_bigint(a);
//     del_si_bigint(b);
// }

Def_TEST(add) {
    si_bigint *a = new_si_bigint_from_num(1);
    // si_bigint *b = new_si_bigint_from_num(UINTMAX_MAX);
    // si_bigint *c = new_si_bigint_from_multi_num_(2, 0, 1);

    si_bigint_add_num(&a, 2);
    assert(si_bigint_eq_num(a, 3));
    // assert(si_bigint_eq(b, c)); // TODO

    del_si_bigint(a);
}

int main() {
    Run_TEST(eq);
    // Run_TEST(not);
    // Run_TEST(and);
    Run_TEST(add);

    return 0;
}
