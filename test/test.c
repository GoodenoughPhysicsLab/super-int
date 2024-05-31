#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define BIGINT_DATA_TYPE uint32_t

#include "../bigint/bigint.h"

void test_0(void)
{
	bigint *t = new_bigint_int(0);
	if (t->data[0] != 0) {
		printf("new_bigint error: t->data[0] = %llu\n", t->data[0]);
		assert(0);
	}

	t = _realloc_bigint(&t, 2);
	if (t->data[0] != 0) {
		printf("_realloc_bigint error: %llu, %llu\n", t->data[0], t->data[1]);
		assert(0);
	}
	
	del_bigint(&t);
	if (t) {
		printf("del_bigint error: &t = %p\n", t);
		assert(0);
	}
}

void test_bigint_shrNum(void) 
{
	BIGINT_DATA_TYPE data[] = {3, 3};
	bigint n = {2, data};
	bigint_shrNum(&n, 1);

	BIGINT_DATA_TYPE tmp = ((BIGINT_DATA_TYPE)1) << (sizeof(BIGINT_DATA_TYPE) * 8 - 1);
	assert(n.data[0] == (1 | tmp));
	assert(n.data[1] == 1);
}

int main(void)
{
	puts("-*- test_bigint start -*-");

	test_bigint_shrNum();

	puts("-*- test_bigint end -*-");
	return 0;
}
