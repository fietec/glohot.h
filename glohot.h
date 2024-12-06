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
typedef void (*GlohotCallback) (void*);

typedef struct{
	int id;
	UINT vk;
	UINT mods;
	GlohotCallback callback;
}GlohotKey;

typedef struct{
	GlohotKey keys[GLOHOT_MAX_KEYS];
	size_t count;
	int id_base;
	uint8_t status;
} Glohot;

Glohot Glohot_create(int id_base, uint8_t flags); // create new Glohot with id_base and flags
void Glohot_add(Glohot *glohot, GlohotKey *key, UINT mods, UINT vk, GlohotCallback callback);
int Glohot_register(Glohot *glohot); // register all added hotkeys, returns 0 on success
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

void Glohot_add(Glohot *glohot, GlohotKey *gk, UINT mods, UINT vk, GlohotCallback callback)
{	
	assert(glohot != NULL && gk != NULL && glohot->count < GLOHOT_MAX_KEYS);
	gk->id = glohot->id_base + glohot->count; // this method is very primative but it will serve for the present
	gk->vk = vk;
	gk->mods = mods;
	gk->callback = callback;
	glohot->keys[glohot->count++] = *gk;
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
		glohot_print("Registering hotkey: {Id:%d, Vk:%u, Mods:%u}\n", key.id, key.vk, key.mods, key.callback);
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
	glohot_print("Listening..\n");
	assert(glohot != NULL);
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
#endif // GLOHOT_IMPLEMENTATION