#include "jat_reader.h"

#include <stdio.h>

#include "jat_platform.h"
#include "jat_reader_file.h"
#include "jat_reader_buffer.h"

enum jat_reader_type {
    JAT_READER_TYPE_FILE = 0,
    JAT_READER_TYPE_BUFFER = 1
};

struct jat_reader {
    enum jat_reader_type type;
    union {
        struct jat_reader_file file_reader;
        struct jat_reader_buffer buffer_reader;
    } real_reader;
};

size_t jat_reader_get_size() {
    return sizeof(struct jat_reader);
}

enum jat_status jat_reader_init_from_file(
    const char* file,
    struct jat_reader *reader
) {
    enum jat_status status;

    reader->type = JAT_READER_TYPE_BUFFER;
    status = jat_reader_file_init(
        file,
        &reader->real_reader.file_reader
    );

    return status;
}

enum jat_status jat_reader_create_from_file(
    const char* file,
    struct jat_reader **reader
) {
    enum jat_status status;
    struct jat_reader *buffer_reader = jat_alloc(
        sizeof(struct jat_reader)
    );

    status = jat_reader_init_from_file(
        file,
        buffer_reader
    );

    if (status != JAT_STATUS_SUCCESS) {
        jat_free(buffer_reader);
        return status;
    }

    *reader = buffer_reader;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_init_from_buffer(
    size_t buffer_size,
    const char* buffer,
    struct jat_reader *reader
) {
    enum jat_status status;

    reader->type = JAT_READER_TYPE_BUFFER;
    status = jat_reader_buffer_init(
        buffer_size,
        buffer,
        &reader->real_reader.buffer_reader
    );

    return status;
}

enum jat_status jat_reader_create_from_buffer(
    size_t buffer_size,
    const char* buffer,
    struct jat_reader **reader
) {
    enum jat_status status;
    struct jat_reader *buffer_reader = jat_alloc(
        sizeof(struct jat_reader)
    );

    status = jat_reader_init_from_buffer(
        buffer_size,
        buffer,
        buffer_reader
    );

    if (status != JAT_STATUS_SUCCESS) {
        jat_free(buffer_reader);
        return status;
    }

    *reader = buffer_reader;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_deinit(
    struct jat_reader *reader
) {
    switch (reader->type) {
        case JAT_READER_TYPE_FILE:
            return jat_reader_file_deinit(
                &reader->real_reader.file_reader
            );
        case JAT_READER_TYPE_BUFFER:
            return jat_reader_buffer_deinit(
                &reader->real_reader.buffer_reader
            );
        default:
            return JAT_STATUS_READER_INVALID_TYPE;
    }
}

enum jat_status jat_reader_destroy(
    struct jat_reader *reader
) {
    enum jat_status status = jat_reader_deinit(reader);

    if (status != JAT_STATUS_SUCCESS) {
        return status;
    }

    jat_free(reader);
    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_reader_peek(
    struct jat_reader *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
) {
    switch (reader->type) {
        case JAT_READER_TYPE_FILE:
            return jat_reader_file_peek(
                &reader->real_reader.file_reader,
                buffer_size,
                buffer,
                read_amount
            );
        case JAT_READER_TYPE_BUFFER:
            return jat_reader_buffer_peek(
                &reader->real_reader.buffer_reader,
                buffer_size,
                buffer,
                read_amount
            );
        default:
            return JAT_STATUS_READER_INVALID_TYPE;
    }
}

enum jat_status jat_reader_consume(
    struct jat_reader *reader,
    size_t buffer_size,
    char *buffer,
    size_t *read_amount
) {
    switch (reader->type) {
        case JAT_READER_TYPE_FILE:
            return jat_reader_file_consume(
                &reader->real_reader.file_reader,
                buffer_size,
                buffer,
                read_amount
            );
        case JAT_READER_TYPE_BUFFER:
            return jat_reader_buffer_consume(
                &reader->real_reader.buffer_reader,
                buffer_size,
                buffer,
                read_amount
            );
        default:
            return JAT_STATUS_READER_INVALID_TYPE;
    }
}

enum jat_status jat_reader_is_eof(
    struct jat_reader *reader,
    bool *is_eof
) {
    switch (reader->type) {
        case JAT_READER_TYPE_FILE:
            return jat_reader_file_is_eof(
                &reader->real_reader.file_reader,
                is_eof
            );
        case JAT_READER_TYPE_BUFFER:
            return jat_reader_buffer_is_eof(
                &reader->real_reader.buffer_reader,
                is_eof
            );
        default:
            return JAT_STATUS_READER_INVALID_TYPE;
    }
}

