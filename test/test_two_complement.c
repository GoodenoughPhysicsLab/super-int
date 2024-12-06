#include <stdint.h>
#include "common.h"

int main() {
    si_bigint *a_ = new_si_bigint_from_multi_num_(-2, UINT64_MAX, UINT64_MAX);
    si_bigint **a = &a_;
    si_bigint_twos_complement(a);
    assert(si_bigint_eq_num(*a, 1));

    return 0;
}
