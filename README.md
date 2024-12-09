# Glohot.h Documentation

**Glohot.h** is a lightweight header-only library for creating global hotkeys on Windows.

## How to use
**Glohot.h** is a stb-style library, which means that besides including `glohot.h` in your project, you first need to include the implementation of all the functions.
This is done as follows:
```c 
#define GLOHOT_IMPLEMENTATION
#include "glohot.h"
```

After including the library, you can easily create custom global hotkeys like this:

```c 
Glohot glohot = Glohot_create(0, GLOHOT_FLAG_DEFAULT);   // initialize Glohot

GlohotKey key1;  // declare hotkey structs
GlohotKey key2;

Glohot_add(&glohot, &key1, MOD_CONTROL, 0x50, &some_callback1);  // populate hotkeys and add them to Glohot
Glohot_add(&glohot, &key2, MOD_CONTROL | MOD_ALT, 0x4F, &some_callback2);  // define modifiers, vk code of the key and the callback function

Glohot_register(&glohot);  // register all hotkeys added to Glohot
Glohot_listen(&glohot);  // listen for key presses
```

As you can see, to initiate a glohot struct, use the Glohot_create(int, uint8_t) function:
Parameters:
- id_base[int] : the id offset to start with
- flags[uint8_t] : behaviour flags

If you want to unregister hotkeys, use `Glohot_unregister(Glohot *glohot, size_t count)`.
The `count` field indicates the amount of hotkeys to unregister starting with index 0.
If you want to unregister all present hotkeys, provided `GLOHOT_ALL`.

### Glohot Utils
The additional header file `glohot_utils.h` contains further definitions and functions for working with global hotkeys.
#### Key Codes
To access vk code and mod defintions define `GLOHOT_KEY_CODES`.
#### Helper Functions
To access helper functions like `emulate_key_press` define `GLOHOT_UTILS`.


## Further remarks

This very simplistic project is in a early state and relies on some quite naive assumptions.
Therefore, please use this project with caution. You are invited to modify, rewrite and publish the source code and create push requests. For more detailed information about copyright see the `LICENSE` file.