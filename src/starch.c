
#include <starch.h>
#include <archive.h>
#include <aco.h>

struct StarchStream_ {
};

StarchStream* starch_init() {
}

void starch_finish(StarchStream* str) {
}

int starch_has_entry(StarchStream* str) {
    return 0;
}

void* starch_next_entry(StarchStream* str) {
    return 0;
}

void starch_feed(StarchStream* str, uint8_t* bytes, size_t len) {
}
