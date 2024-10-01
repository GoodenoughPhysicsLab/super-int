#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "si_bigint.h"

struct si_bigint {
    /* Length of data
     * if si_bigint is negative, len < 0, else len > 0
     * if si_bigint is NaN, len == 0
     */
    intmax_t len;
    uintmax_t data[1];
};

/* [[ Private ]]
 * Calculate the size of a si_bigint
 */
static size_t sizeof_si_bigint_(struct si_bigint const * num) {
    assert(num != NULL);
    return sizeof(struct si_bigint) + (num->len - 1) * sizeof(uintmax_t);
}

/* Create a new si_bigint from a number
 */
struct si_bigint* si_bigint_new_from_num(intmax_t const num) {
    struct si_bigint *res = (struct si_bigint*)malloc(sizeof(struct si_bigint));
    res->len = num < 0 ? -1 : 1;
    res->data[0] = num < 0 ? -num : num;
    return res;
}

/* Create a new si_bigint from a si_bigint
 */
struct si_bigint* si_bigint_new_from_si_bigint(struct si_bigint const * num) {
    assert(num != NULL);

    struct si_bigint *res;
    memcpy(res, num, sizeof_si_bigint_(num));
    return res;
}

/* Reverse every bit in a si_bigint
    @Return: A new si_bigint
 */
struct si_bigint* si_bigint_not(si_bigint const *num) {
    assert(num != NULL);

    struct si_bigint *res = si_bigint_new_from_si_bigint(num);

    if (res->len == 0) { // NaN
        return res;
    }

    for (int i = 0; i < res->len; i++) {
        res->data[i] = ~res->data[i];
    }
    return res;
}

/* Reverse every bit of itself
    @prama num: The si_bigint number to be reversed
    Show error code's meaning in `si_bigint.h`
 */
int si_bigint_self_not(si_bigint *num) {
    assert(num != NULL);

    if (num->len == 0) { // NaN
        return 0;
    }

    for (int i = 0; i < num->len; i++) {
        num->data[i] = ~num->data[i];
    }
    return 0;
}
