#ifndef JAT_READER_H
#define JAT_READER_H

#include <stdbool.h>

#include "jat_status.h"

struct jat_reader;

enum jat_status jat_reader_create_from_file(
    const char* file,
    struct jat_reader **reader
);

enum jat_status jat_reader_create_from_buffer(
    size_t buffer_size,
    const char* buffer,
    struct jat_reader **reader
);

enum jat_status jat_reader_destroy(
    struct jat_reader *reader
);

enum jat_status jat_reader_peek(
    struct jat_reader *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
);

enum jat_status jat_reader_consume(
    struct jat_reader *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
);

enum jat_status jat_reader_is_eof(
    struct jat_reader *reader,
    bool *is_eof
);

#endif
