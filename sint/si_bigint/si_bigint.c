#include <stdint.h>
#include <stdlib.h>
#include "si_bigint.h"

struct si_bigint {
    /* Length of data
     * if si_bigint is negative, len < 0, else len > 0
     * if si_bigint is NaN, len == 0
     */
    intmax_t len;
    uintmax_t data[1];
};

struct si_bigint* si_bigint_new_from_num(intmax_t const num) {
    struct si_bigint *res = (struct si_bigint*)malloc(sizeof(struct si_bigint));
    res->len = num < 0 ? -1 : 1;
    res->data[0] = num < 0 ? -num : num;
    return res;
}
