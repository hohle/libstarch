// Copyright (C) 2019 Jonathan Hohle
#ifndef STARCH_H
#define STARCH_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <archive.h>

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

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef struct StarchStream_ StarchStream;

static const ssize_t SA_E_UNDERFLOW = -(1 << 0);
static const ssize_t SA_E_EOF = -(1 << 1);
static const ssize_t SA_E_ERROR = -(1 << 2);
static const ssize_t SA_E_INTERNAL_ERROR = -(1 << 3);

extern StarchStream* starch_init(struct archive* a)
    __attribute__((nonnull(1)));

extern void starch_finish(StarchStream* str)
    __attribute__((nonnull(1)));

extern bool starch_has_entry(StarchStream* str)
    __attribute__((nonnull(1)));

extern struct archive_entry* starch_next_entry(StarchStream* str)
    __attribute__((nonnull(1)));

extern void starch_feed(StarchStream* str, uint8_t* bytes, size_t len)
    __attribute__((nonnull(1)));

extern ssize_t starch_read(StarchStream* str, uint8_t* buff, size_t len)
    __attribute__((nonnull(1, 2)));

extern void starch_skip(StarchStream* str)
    __attribute__((nonnull(1)));


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // STARCH_H
