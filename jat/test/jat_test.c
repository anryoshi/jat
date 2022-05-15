#include "unity.h"

#include "jat_reader_buffer_test.h"
#include "jat_decoder_test.h"

void setUp(void) {
}

void tearDown(void) {
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(jat_reader_buffer_zero_length_input_test);
    RUN_TEST(jat_reader_buffer_null_input_test);
    RUN_TEST(jat_reader_buffer_valid_data_test);
    RUN_TEST(jat_reader_buffer_peek_invalid_input_test);
    RUN_TEST(jat_reader_buffer_peek_immutability_test);
    RUN_TEST(jat_reader_buffer_consume_invalid_input_test);
    RUN_TEST(jat_reader_buffer_consume_mutability_test);

    RUN_TEST(jat_decoder_smoke_test);

    return UNITY_END();
}
