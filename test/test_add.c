#include "common.h"
#include "si_bigint.h"

int main(void) {
    si_bigint *_1 = new_si_bigint_from_num(1);
    si_bigint **const a = &_1;
    si_bigint *_2 = new_si_bigint_from_multi_num_(1, UINTMAX_MAX);
    si_bigint **const b = &_2;
    si_bigint *_3 = new_si_bigint_from_multi_num_(2, 0, 1);
    si_bigint **const c = &_3;

    si_bigint_add_num(a, 2);
    assert(si_bigint_eq_num(*a, 3));
    si_bigint_add_num(b, 1);
    assert(si_bigint_eq(*b, *c));
    assert(!si_bigint_eq(*a, *b));

    del_si_bigint(*a);
    del_si_bigint(*b);
    del_si_bigint(*c);

    return 0;
}
