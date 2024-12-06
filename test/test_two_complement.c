#include <stdint.h>
#include "common.h"
#include "si_bigint.h"

int main() {
    si_bigint *a = new_si_bigint_from_multi_num_(
        -5, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX
    );

    si_bigint_twos_complement(&a);
    assert(si_bigint_eq_num(a, 1));

    del_si_bigint(a);

    return 0;
}
