#include "common.h"
#include "si_bigint.h"

int main() {
    si_bigint *a = new_si_bigint_from_multi_num_(3, 3, 2, 1);
    si_bigint const*const b = new_si_bigint_from_multi_num_(3, 0, 2, 1);

    si_bigint_sub_num(&a, 3);
    assert(si_bigint_eq(a, b));

    del_si_bigint(a);

    return 0;
}
