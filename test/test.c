#include <stdio.h>
#include "../bigint/bigint.h"

int main(void)
{
#ifdef NDEBUG
    puts("|| release");
#else
    puts("|| debug");
#endif
	puts("-- test_bigint start");

	bigint *t = new_bigint_int(0);
	if (t->data[0] != 0) {
		printf("new_bigint error: t->data[0] = %llu\n", t->data[0]);
		return 0;
	}

	t = _realloc_bigint(&t, 2);
	if (t->data[0] != 0) {
		printf("_realloc_bigint error: %llu, %llu\n", t->data[0], t->data[1]);
		return 0;
	}
	
	del_bigint(&t);
	if (t) {
		printf("del_bigint error: &t = %p\n", t);
		return 0;
	}

	puts("-- test_bigint end");
	return 0;
}
