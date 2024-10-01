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
static size_t sizeof_si_bigint_(struct si_bigint const *num) {
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

/* Create a new si_bigint from a string
 */
struct si_bigint* si_bigint_new_from_str(char const *str) {
    assert(str != NULL);

    // TODO
}

/* Create a new si_bigint from a si_bigint
 */
struct si_bigint* si_bigint_new_from_si_bigint(struct si_bigint const *num) {
    assert(num != NULL);

    struct si_bigint *res;
    memcpy(res, num, sizeof_si_bigint_(num));
    return res;
}

/* Free memory of si_bigint
 */
void si_bigint_del(si_bigint *num) {
    assert(num != NULL);

    free(num);
}

char const* si_bigint_to_str(si_bigint const *num) {
    assert(num != NULL);

    // TODO
}

/* Return true if a si_bigint is NaN
 */
bool si_bigint_is_NaN(si_bigint const *num) {
    return num->len == 0;
}

/* Reverse every bit of itself
 *  @prama num: The si_bigint number to be reversed
 */
void si_bigint_not(si_bigint *num) {
    assert(num != NULL);

    if (si_bigint_is_NaN(num)) {
        return;
    }

    for (int i = 0; i < num->len; i++) {
        num->data[i] = ~num->data[i];
    }
    return;
}

bool si_bigint_eq_num(si_bigint const *num1, intmax_t const num2) {
    assert(num1 != NULL);

    if (si_bigint_is_NaN(num1) || num1->len != 1) {
        return false;
    }
    if (num1->len < 0 && num2 < 0 || num1->len > 0 && num2 >= 0) {
        return num1->data[0] == (uintmax_t)(num2 < 0 ? -num2 : num2);
    }
    return false;
}
