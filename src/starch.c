#include <assert.h>
#include <starch.h>
#include <aco.h>

#define min(a, b) ({ \
    __typeof__(a) _a = (a); \
    __typeof__(b) _b = (b); \
    _a < _b ? _a : _b; \
})

typedef enum ArchiveMode_ {
    PRE_ENTRY,
    PRE_DATA,
    IN_DATA,
    SKIP,
} ArchiveMode;

struct StarchStream_ {
    struct archive* archive;
    ArchiveMode archive_mode;

    uint8_t* buffer;
    size_t buffer_size;

    uint8_t* out;
    size_t out_size;

    bool finished;

    aco_t* main_co;
    aco_t* co;

    bool has_next;
    struct archive_entry* entry;
};


archive_close_callback starch_co_close;

int starch_co_close(__attribute__((unused)) struct archive* a,
                    void* client_data) {
    StarchStream* str = client_data;
    str->finished = true;
    return 0;
}

archive_read_callback starch_co_read;

ssize_t starch_co_read(__attribute__((unused)) struct archive *a,
                       void* client_data,
                       const void **buff) {
    StarchStream* str = client_data;

    while (str->buffer_size == 0 &&
           !str->finished) {
        // yield until there is data available
        fprintf(stderr, "yielding while waiting for a record: %s\n",
                str->finished ? "true" : "false");
        aco_yield();
    }

    *buff = str->buffer;
    size_t size = str->buffer_size;
    str->buffer_size = 0;

    fprintf(stderr, "giving libarchive %zu bytes\n", size);

    assert(size <= SSIZE_MAX);

    return (ssize_t) size;
}

__attribute__((noreturn))
void starch_co() {
    StarchStream* str = aco_get_arg();

    fprintf(stderr, "1: archive_read_open\n");
    archive_read_open(str->archive,
                      str,
                      NULL,
                      starch_co_read,
                      starch_co_close);

    while (true) {
        if (!str->has_next) {
            str->archive_mode = PRE_ENTRY;

            fprintf(stderr, "1: archive_read_next_header\n");
            if (archive_read_next_header(str->archive, &str->entry) != ARCHIVE_OK) {
                fprintf(stderr, "ack!");
            }
            str->archive_mode = PRE_DATA;
            str->has_next = true;
            aco_yield();
        }

        if (str->archive_mode == PRE_DATA ||
            str->archive_mode == IN_DATA) {

            str->archive_mode = IN_DATA;
            str->out_size = 0;
            fprintf(stderr, "2: archive_read_data\n");
            ssize_t len = archive_read_data(str->archive,
                                            str->out,
                                            1 << 13);
            fprintf(stderr, "   = %zu bytes\n", len);
            if (len >= 0) {
                str->out_size = (size_t) len;
                if (len == 0) {
                    str->archive_mode = PRE_ENTRY;
                }
            } else {
                fprintf(stderr, "ack! %zu", len);
            }
            aco_yield();
        }

        if (str->archive_mode == SKIP) {
            archive_read_data_skip(str->archive);
        }
    }

    aco_exit();
}

#pragma mark API

StarchStream* starch_init(struct archive* archive) {
    StarchStream* str = calloc(1, sizeof(StarchStream));
    *str = (StarchStream) {
        .archive = archive,
        .archive_mode = PRE_ENTRY,
        .out = calloc(1 << 13, sizeof(uint8_t)),
        .out_size = 0,
        .has_next = false,
        .entry = NULL,
    };

    str->main_co = aco_create(NULL, NULL, 0, NULL, NULL);
    aco_share_stack_t* sstk = aco_share_stack_new(0);

    str->co = aco_create(str->main_co,
                         sstk,
                         0,
                         starch_co,
                         str);

    // this is expected to return almost immediately after
    // initial  archive setup
    aco_resume(str->co);

    return str;
}

void starch_finish(StarchStream* str) {
    str->finished = true;
    str->buffer = NULL;
    str->buffer_size = 0;

    aco_resume(str->co);

    aco_destroy(str->co);
    aco_destroy(str->main_co);
}

bool starch_has_entry(StarchStream* str) {
    if (str->has_next) {
        return true;
    }
    return str->has_next;
}

struct archive_entry* starch_next_entry(StarchStream* str) {
    fprintf(stderr, "> next entry\n");
    if (str->has_next) {
        struct archive_entry* entry = str->entry;
        str->entry = NULL;
        str->has_next = false;
        aco_resume(str->co);
        return entry;
    }
    return 0;
}

void starch_feed(StarchStream* str, uint8_t* bytes, size_t len) {
    fprintf(stderr, "> feed\n");
    str->buffer = bytes;
    str->buffer_size = len;
    aco_resume(str->co);
}

// TODO: we need to do all the circular buffer crap that comes along
// with readers that might not want all the data
ssize_t starch_read(StarchStream* str, uint8_t* buff, size_t len) {
    if (str->out_size > 0) {
        size_t ret = min(len, str->out_size);
        memcpy(buff, str->out, ret);

        assert(ret <= SSIZE_MAX);

        return (ssize_t) ret;
    }

    fprintf(stderr, "> read\n");
    if (str->finished) {
        fprintf(stderr, "    = EOF\n");
        return SA_E_EOF;
    }

    if (str->archive_mode == PRE_ENTRY) {
        fprintf(stderr, "    = ERROR\n");
        return SA_E_ERROR; // need to call next entry firest
    }

    if (str->archive_mode == PRE_DATA ||
        str->out_size <= 0) {
        fprintf(stderr, "    = UNDERFLOW\n");
        return SA_E_UNDERFLOW;
    }

    if (str->out_size > 0) {
        size_t ret = min(len, str->out_size);
        memcpy(buff, str->out, ret); // TODO: woefully inadequate
        fprintf(stderr, "    = read %zu\n", ret);

        assert(ret <= SSIZE_MAX);

        return (ssize_t) ret;
    }

    fprintf(stderr, "    = INTERNAL ERROR\n");
    return SA_E_INTERNAL_ERROR;
}
