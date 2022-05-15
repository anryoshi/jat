#include "jat_reader_buffer.h"

#include <stddef.h>
#include <string.h>

enum jat_status jat_reader_buffer_init(
    size_t buffer_size,
    const char *buffer,
    struct jat_reader_buffer *reader
) {
    if (buffer_size == 0 || buffer == NULL) {
        return JAT_STATUS_INVALID_ARGUMENTS;
    }

    reader->buffer_size = buffer_size;
    reader->buffer = buffer;
    reader->bytes_read = 0;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_buffer_deinit(
    struct jat_reader_buffer *reader
) {
    reader->buffer_size = 0;
    reader->buffer = NULL;
    reader->bytes_read = 0;
    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_buffer_peek(
    struct jat_reader_buffer *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
) {
    if (buffer_size == 0 || buffer == NULL) {
        return JAT_STATUS_INVALID_ARGUMENTS;
    }

    size_t amount_to_read = 0;
    if (reader->bytes_read + buffer_size <= reader->buffer_size) {
        amount_to_read = buffer_size;
    } else {
        amount_to_read = reader->buffer_size - reader->bytes_read;
    }

    if (amount_to_read != 0) {
        memcpy(buffer, reader->buffer + reader->bytes_read, amount_to_read);
    }

    *read_amount = amount_to_read;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_buffer_consume(
    struct jat_reader_buffer *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
) {
    enum jat_status status;
    status = jat_reader_buffer_peek(
        reader,
        buffer_size,
        buffer,
        read_amount
    );

    if (status != JAT_STATUS_SUCCESS) {
       return status; 
    }

    reader->bytes_read += *read_amount;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_buffer_is_eof(
    struct jat_reader_buffer *reader,
    bool *is_eof
) {
    *is_eof = reader->bytes_read == reader->buffer_size;
    return JAT_STATUS_SUCCESS;
}
