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

Glohot_add(&glohot, GH_MOD_CTRL, GH_VK_P, &some_callback1);  // add hotkeys to Glohot
Glohot_add(&glohot, GH_MOD_CTRL | CH_MOD_ALT, GH_VK_O, &some_callback2);  // provide vk code and modifiers of the key and the callback function

Glohot_register(&glohot);  // register all hotkeys added to Glohot
Glohot_listen(&glohot);  // listen for key presses
```

As you can see, to initiate a glohot struct, use the Glohot_create(int, uint8_t) function:
Parameters:
- id_base[int] : the id offset to start with
- flags[uint8_t] : behaviour flags
	- GLOHOT_FLAG_DEFAULT: the default behaviour
	- GLOHOT_FLAG_NO_UNREG: disable un-registering of all hotkeys after exiting 

A callback function has to be of type `GlohotCallback`, as defined in
```c 
typedef void (*GlohotCallback) (Glohot*);
```
When the assoiciated hotkey is pressed, **Glohot** will call the respective callback function and provide the corresponding **Glohot** instance via the argument.

To gracefully exit `Glohot_listen`, call the `Glohot_exit(Glohot *glohot)` function.
This can be done from a different thread or from a callback funtion of an assigned hotkey.

If you want to unregister hotkeys, use `Glohot_unregister(Glohot *glohot, size_t count)`.
The `count` field indicates the amount of hotkeys to unregister starting with index 0.
If you want to unregister all present hotkeys, provided `GLOHOT_ALL`.


To disable non-error console output from **Glohot**, define `GLOHOT_SILENT`.

### Key Codes
To access vk code and mod definitions define `GLOHOT_KEY_CODES`.
### Helper utils
To access helper functions like `emulate_key_press` define `GLOHOT_UTILS`.


## Further remarks

This very simplistic project is in a early state and relies on some quite naive assumptions.
Therefore, please use this project with caution. You are invited to modify, rewrite and publish the source code and create push requests. For more detailed information about copyright see the `LICENSE` file.