#ifndef _GLOHOT_UTILS_H
#define _GLOHOT_UTILS_H

#ifdef GLOHOT_KEY_CODES

#define GH_MOD_CTRL         0x01
#define GH_MOD_ALT          0x02

// Mouse buttons
#define GH_VK_LBUTTON       0x01
#define GH_VK_RBUTTON       0x02
#define GH_VK_CANCEL        0x03
#define GH_VK_MBUTTON       0x04
#define GH_VK_XBUTTON1      0x05
#define GH_VK_XBUTTON2      0x06

// Control keys
#define GH_VK_BACK          0x08
#define GH_VK_TAB           0x09
#define GH_VK_CLEAR         0x0C
#define GH_VK_RETURN        0x0D
#define GH_VK_SHIFT         0x10
#define GH_VK_CONTROL       0x11
#define GH_VK_MENU          0x12
#define GH_VK_PAUSE         0x13
#define GH_VK_CAPITAL       0x14
#define GH_VK_KANA          0x15
#define GH_VK_HANGUL        0x15
#define GH_VK_IME_ON        0x16
#define GH_VK_JUNJA         0x17
#define GH_VK_FINAL         0x18
#define GH_VK_HANJA         0x19
#define GH_VK_KANJI         0x19
#define GH_VK_IME_OFF       0x1A
#define GH_VK_ESCAPE        0x1B
#define GH_VK_CONVERT       0x1C
#define GH_VK_NONCONVERT    0x1D
#define GH_VK_ACCEPT        0x1E
#define GH_VK_MODECHANGE    0x1F
#define GH_VK_SPACE         0x20

// Navigation keys
#define GH_VK_PRIOR         0x21
#define GH_VK_NEXT          0x22
#define GH_VK_END           0x23
#define GH_VK_HOME          0x24
#define GH_VK_LEFT          0x25
#define GH_VK_UP            0x26
#define GH_VK_RIGHT         0x27
#define GH_VK_DOWN          0x28

// Miscellaneous
#define GH_VK_SELECT        0x29
#define GH_VK_PRINT         0x2A
#define GH_VK_EXECUTE       0x2B
#define GH_VK_SNAPSHOT      0x2C
#define GH_VK_INSERT        0x2D
#define GH_VK_DELETE        0x2E
#define GH_VK_HELP          0x2F

// Number keys
#define GH_VK_0             0x30
#define GH_VK_1             0x31
#define GH_VK_2             0x32
#define GH_VK_3             0x33
#define GH_VK_4             0x34
#define GH_VK_5             0x35
#define GH_VK_6             0x36
#define GH_VK_7             0x37
#define GH_VK_8             0x38
#define GH_VK_9             0x39

// Alphabet keys
#define GH_VK_A             0x41
#define GH_VK_B             0x42
#define GH_VK_C             0x43
#define GH_VK_D             0x44
#define GH_VK_E             0x45
#define GH_VK_F             0x46
#define GH_VK_G             0x47
#define GH_VK_H             0x48
#define GH_VK_I             0x49
#define GH_VK_J             0x4A
#define GH_VK_K             0x4B
#define GH_VK_L             0x4C
#define GH_VK_M             0x4D
#define GH_VK_N             0x4E
#define GH_VK_O             0x4F
#define GH_VK_P             0x50
#define GH_VK_Q             0x51
#define GH_VK_R             0x52
#define GH_VK_S             0x53
#define GH_VK_T             0x54
#define GH_VK_U             0x55
#define GH_VK_V             0x56
#define GH_VK_W             0x57
#define GH_VK_X             0x58
#define GH_VK_Y             0x59
#define GH_VK_Z             0x5A

// Windows keys
#define GH_VK_LWIN          0x5B
#define GH_VK_RWIN          0x5C
#define GH_VK_APPS          0x5D

// Numpad keys
#define GH_VK_NUMPAD0       0x60
#define GH_VK_NUMPAD1       0x61
#define GH_VK_NUMPAD2       0x62
#define GH_VK_NUMPAD3       0x63
#define GH_VK_NUMPAD4       0x64
#define GH_VK_NUMPAD5       0x65
#define GH_VK_NUMPAD6       0x66
#define GH_VK_NUMPAD7       0x67
#define GH_VK_NUMPAD8       0x68
#define GH_VK_NUMPAD9       0x69
#define GH_VK_MULTIPLY      0x6A
#define GH_VK_ADD           0x6B
#define GH_VK_SEPARATOR     0x6C
#define GH_VK_SUBTRACT      0x6D
#define GH_VK_DECIMAL       0x6E
#define GH_VK_DIVIDE        0x6F

// Function keys
#define GH_VK_F1            0x70
#define GH_VK_F2            0x71
#define GH_VK_F3            0x72
#define GH_VK_F4            0x73
#define GH_VK_F5            0x74
#define GH_VK_F6            0x75
#define GH_VK_F7            0x76
#define GH_VK_F8            0x77
#define GH_VK_F9            0x78
#define GH_VK_F10           0x79
#define GH_VK_F11           0x7A
#define GH_VK_F12           0x7B
#define GH_VK_F13           0x7C
#define GH_VK_F14           0x7D
#define GH_VK_F15           0x7E
#define GH_VK_F16           0x7F
#define GH_VK_F17           0x80
#define GH_VK_F18           0x81
#define GH_VK_F19           0x82
#define GH_VK_F20           0x83
#define GH_VK_F21           0x84
#define GH_VK_F22           0x85
#define GH_VK_F23           0x86
#define GH_VK_F24           0x87

// Lock keys
#define GH_VK_NUMLOCK       0x90
#define GH_VK_SCROLL        0x91

// OEM and Misc keys
#define GH_VK_OEM_1         0xBA
#define GH_VK_OEM_PLUS      0xBB
#define GH_VK_OEM_COMMA     0xBC
#define GH_VK_OEM_MINUS     0xBD
#define GH_VK_OEM_PERIOD    0xBE
#define GH_VK_OEM_2         0xBF
#define GH_VK_OEM_3         0xC0
#define GH_VK_OEM_4         0xDB
#define GH_VK_OEM_5         0xDC
#define GH_VK_OEM_6         0xDD
#define GH_VK_OEM_7         0xDE
#define GH_VK_OEM_CLEAR     0xFE

#endif // GLOHOT_VK_CODES
#ifdef GLOHOT_UTILS
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
#endif // GLOHOT_UTILS
#endif // _GLOHOT_UTILS_H