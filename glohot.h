#ifndef _GLOHOT_H
#define _GLOHOT_H

#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <stdint.h>

#define GLOHOT_MAX_KEYS 32
#define GLOHOT_ALL -1

// flags
#define GLOHOT_FLAG_DEFAULT  0x0
#define GLOHOT_FLAG_RUNNING  0x1
#define GLOHOT_FLAG_NO_UNREG 0x2

#ifndef GLOHOT_SILENT
	#define glohot_print(msg, ...) (printf((msg), ##__VA_ARGS__))
#else
	// only error messages
	#define glohot_print(msg, ...) 
#endif

typedef struct Glohot Glohot;
typedef void (*GlohotCallback) (Glohot*);

typedef struct{
	int id;
	UINT vk;
	UINT mods;
	GlohotCallback callback;
} GlohotKey;

struct Glohot{
	GlohotKey keys[GLOHOT_MAX_KEYS];
	size_t count;
	int id_base;
	uint8_t status;
};

Glohot Glohot_create(int id_base, uint8_t flags); // create new Glohot with id_base and flags
void Glohot_add(Glohot *glohot, UINT mods, UINT vk, GlohotCallback callback);
int  Glohot_register(Glohot *glohot); // register all added hotkeys, returns 0 on success
void Glohot_unregister(Glohot *glohot, size_t count); // unregiser n-hotkeys of the given Glohot
void Glohot_listen(Glohot *glohot); // runs the mein listener loop, exit via Ctrl-C or Gloht_exit()
void Glohot_exit(Glohot *glohot); // terminates the running listener loop of the given Glohot
void Glohot_PrintLastError(); // just a helper function, prints the last win-api error

#endif // _GLOHOT_H

#ifdef GLOHOT_IMPLEMENTATION

Glohot Glohot_create(int id_base, uint8_t flags)
{
	return (Glohot) {{0}, 0, id_base, flags};	
}

void Glohot_add(Glohot *glohot, UINT mods, UINT vk, GlohotCallback callback)
{	
	assert(glohot != NULL && glohot->count < GLOHOT_MAX_KEYS);
	GlohotKey gk;
	gk.id = glohot->id_base + glohot->count; // this method is very primative but it will serve for the present
	gk.vk = vk;
	gk.mods = mods;
	gk.callback = callback;
	glohot->keys[glohot->count++] = gk;
}

void Glohot_unregister(Glohot *glohot, size_t count)
{	
	assert(glohot != NULL);
	if (count == GLOHOT_ALL) count = glohot->count;
	for (size_t i=0; i<count; ++i){
		GlohotKey key = glohot->keys[i];
		if (UnregisterHotKey(NULL, key.id) == 0){
			fprintf(stderr, "[ERROR] Failed to unregister hotkey %u!\n", key.id);
		}
	}
}

int Glohot_register(Glohot *glohot)
{
	assert(glohot != NULL);
	for (size_t i=0; i<glohot->count; ++i){
		GlohotKey key = glohot->keys[i];
		glohot_print("Registering hotkey: {Id:%d, Vk:%u, Mods:%u}\n", key.id, key.vk, key.mods);
		if (RegisterHotKey(NULL, key.id, key.mods, key.vk) == 0){
			fprintf(stderr, "[ERROR] Could not register hotkey %u: ", key.id);
			Glohot_PrintLastError();
			Glohot_unregister(glohot, i);
			return 1;
		}
	}
	return 0;
}

GlohotCallback Glohot_get(Glohot *glohot, int id)
{
	assert(glohot != NULL);
	for (size_t i=0; i<glohot->count; ++i){
		GlohotKey key = glohot->keys[i];
		if (key.id == id) return key.callback;
	}
	return NULL;
}

void Glohot_exit(Glohot *glohot)
{
	if (glohot == NULL) return;
	glohot->status &= ~GLOHOT_FLAG_RUNNING;
}

void Glohot_listen(Glohot *glohot)
{
	assert(glohot != NULL);
	glohot_print("Listening..\n");
	MSG msg = {0};
	glohot->status |= GLOHOT_FLAG_RUNNING;
	while ((glohot->status & GLOHOT_FLAG_RUNNING) != 0 && GetMessage(&msg, NULL, 0, 0) != 0){
		if (msg.message == WM_HOTKEY){
			GlohotCallback callback = Glohot_get(glohot, msg.wParam);
			if (callback != NULL){
				callback(glohot);
			}
		}
	}
	if ((glohot->status & GLOHOT_FLAG_NO_UNREG) == 0){
		Glohot_unregister(glohot, GLOHOT_ALL);
	}
	glohot_print("Exiting..\n");
}

void Glohot_PrintLastError() 
{ 
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );
	printf(lpMsgBuf);
	putchar('\n');
    LocalFree(lpMsgBuf);
}

#ifdef GLOHOT_KEY_CODES
/* 
	These defintions are mostly renamed versions of the vk codes found in winuser.h.
	Further codes as e.g. for digits and alphabetical characters have been added for consitency.
*/

// Key modifiers
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
#define GH_VK_LSHIFT 	    0xA0
#define GH_VK_RSHIFT 	    0xA1 	
#define GH_VK_LCONTROL 	    0xA2 	
#define GH_VK_RCONTROL 	    0xA3 	
#define GH_VK_LMENU 	    0xA4 	
#define GH_VK_RMENU 	    0xA5 	
#define GH_VK_BROWSER_BACK 	0xA6 	
#define GH_VK_BROWSER_FWD 	0xA7 	
#define GH_VK_BROWSER_RFRS	0xA8 	
#define GH_VK_BROWSER_STOP 	0xA9 	
#define GH_VK_BROWSER_SRCH  0xAA 	
#define GH_VK_BROWSER_FAVS 	0xAB 	
#define GH_VK_BROWSER_HOME 	0xAC 	
#define GH_VK_VOLUME_MUTE 	0xAD 
#define GH_VK_VOLUME_DOWN 	0xAE 	
#define GH_VK_VOLUME_UP 	0xAF 	
#define GH_VK_MEDIA_NEXT 	0xB0 	
#define GH_VK_MEDIA_PREV 	0xB1 	
#define GH_VK_MEDIA_STOP 	0xB2 	
#define GH_VK_MEDIA_PLAY 	0xB3 // originally VK_MEDIA_PLAY_PAUSE, shortened for consistency

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
/*
	Some functions for working with hotkeys on Windows. 
	These are not per se related to `glohot` itself, but are often used together, which is why I decided to provide them here. 
*/

// emulate a key press, useful when trying to achieve behaviour of keys not present on some keyboards, e.g. GH_VK_MEDIA_PLAY, etc. 
void emulate_key_press(UINT vk)
{
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));
    if (vk == 0x01 || vk == 0x02 || vk == 0x04 || vk == 0x05) {
        // Handle mouse button events
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = (vk == 0x01) ? MOUSEEVENTF_LEFTDOWN :
                               (vk == 0x02) ? MOUSEEVENTF_RIGHTDOWN :
                               (vk == 0x04) ? MOUSEEVENTF_MIDDLEDOWN : 0;
        
        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dwFlags = (vk == 0x01) ? MOUSEEVENTF_LEFTUP :
                               (vk == 0x02) ? MOUSEEVENTF_RIGHTUP :
                               (vk == 0x04) ? MOUSEEVENTF_MIDDLEUP : 0;

    }else {
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = vk;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = vk;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    }

    UINT uSent = SendInput(2, inputs, sizeof(INPUT));
}
#endif // GLOHOT_UTILS
#endif // GLOHOT_IMPLEMENTATION