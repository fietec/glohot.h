#ifndef _GLOHOT_H
#define _GLOHOT_H

#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <stdint.h>

#define GLOHOT_MAX_KEYS 32
#define GLOHOT_ALL 0
#define GLOHOT_RUNNING 1

#ifndef GLOHOT_SILENT
	#define glohot_print(msg, ...) (printf((msg), ##__VA_ARGS__))
#else
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
	uint8_t status;
} Glohot;

void Glohot_init(Glohot *glohot);
void Glohot_add(Glohot *glohot, GlohotKey *key, UINT mods, UINT vk, GlohotCallback callback);
int Glohot_register(Glohot *glohot);
void Glohot_unregister(Glohot *glohot, size_t count);
void Glohot_listen(Glohot *glohot);
void Glohot_PrintLastError();

#endif // _GLOHOT_H

#ifdef GLOHOT_IMPLEMENTATION
void Glohot_add(Glohot *glohot, GlohotKey *gk, UINT mods, UINT vk, GlohotCallback callback)
{	
	assert(glohot != NULL && gk != NULL && glohot->count < GLOHOT_MAX_KEYS);
	gk->id = glohot->count; // this method is very primative but it will serve for the present
	gk->vk = vk;
	gk->mods = mods;
	gk->callback = callback;
	glohot->keys[glohot->count++] = *gk;
}

void Glohot_init(Glohot *glohot)
{
	assert(glohot != NULL);
	memset(glohot, 0, sizeof(Glohot));
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
	glohot->status &= ~GLOHOT_RUNNING;
}

void Glohot_listen(Glohot *glohot)
{
	glohot_print("Listening..\n");
	assert(glohot != NULL);
	MSG msg = {0};
	glohot->status |= GLOHOT_RUNNING;
	while ((glohot->status & GLOHOT_RUNNING) != 0 && GetMessage(&msg, NULL, 0, 0) != 0){
		if (msg.message == WM_HOTKEY){
			GlohotCallback callback = Glohot_get(glohot, msg.wParam);
			if (callback != NULL){
				callback(glohot);
			}
		}
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