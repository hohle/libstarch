
#ifndef STARCH_H
#define STARCH_H

#include <stdint.h>
#include <stddef.h>

/**
 * `libstarch` is a non-blocking wrapper over `libarchive` with an API designed
 * for use with evented/async/non-blocking IO.
 *
 * When using event oriented frameworks (e.g. `libevent`, `libev`, `libuv`,
 * Cocoa, etc.) the event loop "drives" top level control of a program by
 * executing user defined callbacks when configured events occur. `libarchive`
 * also wants to "drive" and expects IO to occur within its scope making it
 * difficult to use with non-blocking IO. `libstarch` abstracts everything
 * necessary to be able to use `libarchive` along with the non-blocking IO
 * framework of your choice.
 */

typedef struct StarchStream_ StarchStream;

extern StarchStream* starch_init();
extern void starch_finish(StarchStream* str);
extern int starch_has_entry(StarchStream* str);
extern void* starch_next_entry(StarchStream* str);
extern void starch_feed(StarchStream* str, uint8_t* bytes, size_t len);

#endif // STARCH_H
