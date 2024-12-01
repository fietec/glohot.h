#ifndef _GLOHOT_H
#define _GLOHOT_H

#include <stdio.h>
#include <assert.h>
#include <windows.h>

#define GLOHOT_MAX_KEYS 32
#define GLOHOT_ALL 0

typedef void (*GlohotCallback) (void);

typedef struct{
	int id;
	UINT vk;
	UINT mods;
	GlohotCallback callback;
}GlohotKey;

typedef struct{
	GlohotKey keys[GLOHOT_MAX_KEYS];
	size_t count;
} Glohot;

void Glohot_init(Glohot *glohot);
void Glohot_add(Glohot *glohot, GlohotKey *key, UINT mods, UINT vk, GlohotCallback callback);
int Glohot_register(Glohot *glohot);
void Glohot_unregister(Glohot *glohot, size_t count);
void Glohot_listen(Glohot *glohot);

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
		printf("Trying to register {%d, %u, %u, %p}\n", key.id, key.vk, key.mods, key.callback);
		if (RegisterHotKey(NULL, key.id, key.mods, key.vk) == 0){
			fprintf(stderr, "[ERROR] Could not register hotkey %u!\n", key.id);
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

void Glohot_listen(Glohot *glohot)
{
	assert(glohot != NULL);
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) != 0){
		if (msg.message == WM_HOTKEY){
			GlohotCallback callback = Glohot_get(glohot, msg.wParam);
			if (callback != NULL){
				callback();
			}
		}
	}
}
#endif // GLOHOT_IMPLEMENTATION