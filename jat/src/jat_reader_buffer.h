#ifndef JAT_READER_BUFFER_H
#define JAT_READER_BUFFER_H

#include <stdbool.h>

#include "jat_status.h"

struct jat_reader_buffer {
    size_t buffer_size;
    const char *buffer;
    size_t bytes_read;
};

enum jat_status jat_reader_buffer_init(
    size_t buffer_size,
    const char *buffer,
    struct jat_reader_buffer *reader
);

enum jat_status jat_reader_buffer_deinit(
    struct jat_reader_buffer *reader
);

enum jat_status jat_reader_buffer_peek(
    struct jat_reader_buffer *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
);

enum jat_status jat_reader_buffer_consume(
    struct jat_reader_buffer *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
);

enum jat_status jat_reader_buffer_is_eof(
    struct jat_reader_buffer *reader,
    bool *is_eof
);

#endif
