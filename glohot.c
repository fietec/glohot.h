#include "glohot.h"

void p(void)
{
	printf("Hello World\n");
}

void o(void)
{
	printf("pressed Ctrl-Alt-O\n");
}

void test(void)
{
	printf("not implemented, yet\n");
}

int main(void)
{
	Glohot glohot;
	Glohot_init(&glohot);
	
	GlohotKey key1;
	GlohotKey key2;
	GlohotKey key3;
	Glohot_add(&glohot, &key1, 0x50, MOD_CONTROL, &p);
	Glohot_add(&glohot, &key2, 0x4F, MOD_CONTROL | MOD_ALT, &o);
	Glohot_add(&glohot, &key3, 0x76, MOD_CONTROL | MOD_ALT, &test);
	
	Glohot_register(&glohot);
	Glohot_listen(&glohot);
	return 0;
}