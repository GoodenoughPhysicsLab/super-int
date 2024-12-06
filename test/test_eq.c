#include "common.h"

int main(void) {
    si_bigint *a = new_si_bigint_from_num(1);
    si_bigint *b = new_si_bigint_from_num(1);
    si_bigint *c = new_si_bigint_from_num(-1);

    assert(si_bigint_eq_num(a, 1));
    assert(!si_bigint_eq_num(a, 2));
    assert(si_bigint_eq_num(c, -1));
    assert(!si_bigint_eq_num(c, -2));
    assert(si_bigint_eq(a, b));
    assert(!si_bigint_eq(a, c));

    del_si_bigint(a);
    del_si_bigint(b);
    del_si_bigint(c);

    return 0;
}
