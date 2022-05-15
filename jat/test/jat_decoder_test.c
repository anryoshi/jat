#include "jat_decoder_test.h"
#include "jat_decoder.h"
#include "unity.h"

#include "string.h"
#include "stdlib.h"

void jat_decoder_smoke_test(void) {
    const char *input = "Hello, World!";
    size_t input_size = strlen(input);

    enum jat_status status;
    struct jat_reader *reader;

    status = jat_reader_create_from_buffer(
        input_size,
        input,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);

    struct jat_decoder *decoder;
    status = jat_decoder_create(
        reader,
        255,
        255,
        &decoder
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);

    uint32_t *out = malloc(input_size * sizeof(uint32_t));
    size_t runes_read;

    status = jat_decoder_consume(
        decoder,
        input_size,
        out,
        &runes_read
    );
    
    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
}
