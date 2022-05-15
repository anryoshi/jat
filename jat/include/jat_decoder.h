#ifndef JAT_DECODER_H
#define JAT_DECODER_H

#include <stddef.h>
#include <stdint.h>

#include "jat_status.h"
#include "jat_reader.h"

struct jat_decoder;

enum jat_status jat_decoder_create(
    struct jat_reader *reader,
    size_t input_buffer_size,
    size_t output_cache_size,
    struct jat_decoder **decoder
);

size_t jat_decoder_get_size(
    size_t input_buffer_size,
    size_t output_cache_size
);

enum jat_status jat_decoder_init(
    struct jat_reader *reader,
    size_t input_buffer_size,
    size_t output_cache_size,
    struct jat_decoder *decoder
);

enum jat_status jat_decoder_deinit(
    struct jat_decoder *decoder
);

enum jat_status jat_decoder_destroy(
    struct jat_decoder *decoder
);

/* enum jat_status jat_decoder_peek( */
/*     struct jat_decoder *decoder, */
/*     size_t buffer_size, */
/*     uint32_t *buffer, */
/*     size_t *runes_read */
/* ); */

enum jat_status jat_decoder_consume(
    struct jat_decoder *decoder,
    size_t buffer_size,
    uint32_t *buffer,
    size_t *runes_read
);

enum jat_status jat_decoder_is_eof(
    struct jat_decoder *decoder
);

#endif // JAT_DECODER_H
