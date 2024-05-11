#include <assert.h>
#include <stdio.h>
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

int main(void)
{
#ifdef NDEBUG
    puts("-*- release -*-");
#else
    puts("-*- debug -*-");
#endif
	puts("-- test_bigint start");

	test_0();

	puts("-- test_bigint end");
	return 0;
}
