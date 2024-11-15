#ifndef _GLOHOT_UTILS_H
#define _GLOHOT_UTILS_H
#include <windows.h>

void emulate_key_press(UINT vk)
{
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vk;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = vk;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(2, inputs, sizeof(INPUT));
}
#endif // _GLOHOT_UTILS_H