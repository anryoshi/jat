#include "jat_reader_file.h"

enum jat_status jat_reader_file_init(
    const char *file,
    struct jat_reader_file *reader
) {
    (void) file;
    (void) reader;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_file_deinit(
    struct jat_reader_file *reader
) {
    (void) reader;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_file_peek(
    struct jat_reader_file *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
) {
    (void) reader;
    (void) buffer_size;
    (void) buffer;
    (void) read_amount;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_file_consume(
    struct jat_reader_file *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
) {
    (void) reader;
    (void) buffer_size;
    (void) buffer;
    (void) read_amount;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_file_is_eof(
    struct jat_reader_file *reader,
    bool *is_eof
) {
    (void) reader;
    (void) is_eof;

    return JAT_STATUS_SUCCESS;
}
