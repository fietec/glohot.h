#include <stdio.h>
#define GLOHOT_IMPLEMENTATION
#define GLOHOT_KEY_CODES
#include "glohot.h"

void test(Glohot *glohot)
{
    (void*) glohot;
    printf("Hotkey pressed!\n");
}

void stop(Glohot *glohot)
{
    Glohot_exit(glohot);
}

int main(void)
{
    Glohot glohot = Glohot_create(0, 0);
    
    Glohot_add(&glohot, GH_MOD_CTRL | GH_MOD_ALT, GH_VK_RETURN, &test);
    Glohot_add(&glohot, GH_MOD_CTRL | GH_MOD_ALT, GH_VK_ESCAPE, &stop);
    
    if (Glohot_register(&glohot) != 0) return 1;
    Glohot_listen(&glohot);
    return 0;
}