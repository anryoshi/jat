#include "jat_decoder.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "jat_platform.h"
#include "jat_utf8_decoder.h"

struct jat_buffer_state {
    size_t size;
    size_t read_offset;
    size_t write_offset;
};

static inline
void jat_buffet_state_init(
    struct jat_buffer_state *buffer_state,
    size_t size
) {
    buffer_state->size = size;
    buffer_state->read_offset = 0;
    buffer_state->write_offset = 0;
}

struct jat_decoder {
    struct jat_reader *reader;
    struct jat_buffer_state input_state;
    struct jat_buffer_state cache_state;
    uint32_t decoder_state;
};

size_t jat_decoder_get_size(
    size_t input_buffer_size,
    size_t output_cache_size
) {
    return sizeof(struct jat_decoder)
        + sizeof(char) * input_buffer_size
        + sizeof(uint32_t) * output_cache_size;
}

static inline
char *jat_decoder_get_input_buffer(
    struct jat_decoder *decoder
) {
    return (char*)(
        (char*)decoder
        + sizeof(struct jat_decoder)
    );
}

static inline
uint32_t *jat_decoder_get_output_cache(
    struct jat_decoder *decoder
) {
    return (uint32_t*)(
        (char*)decoder
        + sizeof(struct jat_decoder)
        + sizeof(char) * decoder->input_state.size
    );
}

enum jat_status jat_decoder_create(
    struct jat_reader *reader,
    size_t input_buffer_size,
    size_t output_cache_size,
    struct jat_decoder **decoder
) {
    enum jat_status status;
    struct jat_decoder *result = jat_alloc(
        jat_decoder_get_size(
            input_buffer_size,
            output_cache_size
        )
    );

    status = jat_decoder_init(
        reader,
        input_buffer_size,
        output_cache_size,
        result
    );

    if (status != JAT_STATUS_SUCCESS) {
        jat_free(result);
        return status;
    }

    *decoder = result;

    return JAT_STATUS_SUCCESS;
}


enum jat_status jat_decoder_init(
    struct jat_reader *reader,
    size_t input_buffer_size,
    size_t output_cache_size,
    struct jat_decoder *decoder
) {
    if (input_buffer_size < 4 || output_cache_size < 1) {
        return JAT_STATUS_INVALID_ARGUMENTS;
    }

    decoder->reader = reader;

    jat_buffet_state_init(
        &decoder->input_state,
        input_buffer_size
    );

    jat_buffet_state_init(
        &decoder->cache_state,
        output_cache_size
    );

    decoder->decoder_state = UTF8_ACCEPT;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_decoder_deinit(
    struct jat_decoder *decoder
) {
    decoder->reader = NULL;
    /* decoder->input_buffer_size = 0; */
    /* decoder->output_cache_size = 0; */

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_decoder_destroy(
    struct jat_decoder *decoder
) {
    enum jat_status status;
    status = jat_decoder_deinit(
        decoder
    );

    if (status != JAT_STATUS_SUCCESS) {
        return status;
    }

    jat_free(decoder);
    return JAT_STATUS_SUCCESS;
}

static inline
bool jat_decoder_is_input_read(
    struct jat_decoder *d
) {
    return d->input_state.read_offset == d->input_state.write_offset;
}

static inline
enum jat_status jat_decoder_fill_input_buffer(
    struct jat_decoder *d
) {
    if (d->input_state.read_offset != d->input_state.write_offset) {
        return JAT_STATUS_DECODER_INTERNAL_ERROR;
    }

    d->input_state.read_offset = 0;
    return jat_reader_consume(
        d->reader,
        d->input_state.size,
        jat_decoder_get_input_buffer(d),
        &d->input_state.write_offset
    );
}

static inline
bool jat_decoder_is_cache_read(
    struct jat_decoder *d
) {
    return d->cache_state.read_offset == d->cache_state.write_offset;
}

static inline
bool jat_decoder_is_cache_full(
    struct jat_decoder *d
) {
    return d->cache_state.write_offset == d->cache_state.size;
}

static inline
bool jat_decoder_is_fully_read(
    struct jat_decoder *d
) {
    bool is_eof;
    enum jat_status status = jat_reader_is_eof(
        d->reader,
        &is_eof
    );

    if (status != JAT_STATUS_SUCCESS) {
        // TODO: Handle this in better way
        return true;
    }

    return is_eof
        && jat_decoder_is_input_read(d)
        && jat_decoder_is_cache_read(d);
}

static inline
enum jat_status jat_decoder_decode_input(
    struct jat_decoder *d
) {
    size_t free_cache_space = d->cache_state.size - d->cache_state.write_offset;
    size_t undecoded_input_bytes = d->input_state.write_offset - d->input_state.read_offset;

    if (undecoded_input_bytes == 0 || free_cache_space == 0) {
        return JAT_STATUS_SUCCESS; 
    }

    char *input_buffer = jat_decoder_get_input_buffer(d);
    uint32_t *cache    = jat_decoder_get_output_cache(d);

    for (;d->input_state.read_offset != d->input_state.write_offset
          && d->cache_state.write_offset != d->cache_state.size
         ;++d->input_state.read_offset
    ) {
        uint32_t rune;
        uint32_t new_state = jat_utf8_decode(
            d->decoder_state,
            *(input_buffer + d->input_state.read_offset),
            &rune
        );

        switch (new_state) {
            case UTF8_ACCEPT:
                *(cache + (d->cache_state.write_offset++)) = rune;
                break;
            case UTF8_REJECT:
                // TODO: Find better handling for this thing
                return JAT_STATUS_DECODER_INVALID_UTF8;
            default:
                d->decoder_state = new_state;
                break;
        }
    }

    return JAT_STATUS_SUCCESS;
}

/*
 * Populate/Shift cycle
 * NOTE: Because caller could provide big buffer with size >> than cache size
 *       and assuming that error from reader could be unrecoverable
 *       we always pass current decoding result to the out buffer
 *
 * TODO: What is good arguments to balance read/decode cycle?
 *
 * Task populate cache fully
 *
 * 1. if input_buffer contains undecoded runes trying to decode
 *    until empty of undecodeable tail ('empty')
 * 2. Fully populate input_buffer with new data from reader
 * 3. Decode till input_buffer is 'empty' or cache is full
 * 4. If cache is full break the cycle
 * 5. If buffer is empty repeat from step 2
 */
static inline
enum jat_status jat_decoder_populate_cache(
    struct jat_decoder *decoder
) {
    /*
     * Invariant of the cache - almost always should be empty
     * => (write_offset == 0)
     */
    enum jat_status status;

    if (!jat_decoder_is_input_read(decoder)) {
        status = jat_decoder_decode_input(decoder);
        if (status != JAT_STATUS_SUCCESS) {
            return status; 
        }
    }

    while (!jat_decoder_is_cache_full(decoder)) {
        status = jat_decoder_fill_input_buffer(decoder);
        if (status != JAT_STATUS_SUCCESS) {
            return status; 
        }

        if (decoder->input_state.write_offset == 0) {
            break;
        }

        status = jat_decoder_decode_input(decoder);
        if (status != JAT_STATUS_SUCCESS) {
            return status; 
        }
    }
    
    return JAT_STATUS_SUCCESS;
}

static inline
enum jat_status jat_decoder_clear_cache(
    struct jat_decoder *decoder
) {
    if (decoder->cache_state.read_offset != decoder->cache_state.write_offset) {
        return JAT_STATUS_DECODER_INTERNAL_ERROR;
    }
    
    decoder->cache_state.read_offset = 0;
    decoder->cache_state.write_offset = 0;

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_decoder_consume(
    struct jat_decoder *decoder,
    size_t buffer_size,
    uint32_t *buffer,
    size_t *runes_read
) {
    uint32_t *cache = jat_decoder_get_output_cache(decoder);

    *runes_read = 0;

    for (; *runes_read != buffer_size && !jat_decoder_is_fully_read(decoder)
         ; ++(*runes_read), ++decoder->cache_state.read_offset
    ) {
        if (jat_decoder_is_cache_read(decoder)) {
            enum jat_status status;
            status = jat_decoder_clear_cache(decoder);
            if (status != JAT_STATUS_SUCCESS) {
                return status;
            }

            status = jat_decoder_populate_cache(decoder);
            if (status != JAT_STATUS_SUCCESS) {
                return status;
            }
        }
        
        *(buffer + *runes_read) = *(cache + decoder->cache_state.read_offset);
    }

    return JAT_STATUS_SUCCESS;
}

enum jat_status jat_decoder_is_eof(
    struct jat_decoder *decoder
);
