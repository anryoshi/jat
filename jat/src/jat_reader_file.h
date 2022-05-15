#ifndef JAT_READER_FILE_H
#define JAT_READER_FILE_H

#include <stdbool.h>
#include <stdio.h>

#include "jat_status.h"

struct jat_reader_file {
    FILE* f;
};

enum jat_status jat_reader_file_init(
    const char *file,
    struct jat_reader_file *reader
);

enum jat_status jat_reader_file_deinit(
    struct jat_reader_file *reader
);

enum jat_status jat_reader_file_peek(
    struct jat_reader_file *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
);

enum jat_status jat_reader_file_consume(
    struct jat_reader_file *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
);

enum jat_status jat_reader_file_is_eof(
    struct jat_reader_file *reader,
    bool *is_eof
);

#endif
