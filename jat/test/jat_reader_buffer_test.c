#include "jat_reader_buffer_test.h"
#include "jat_reader_buffer.h"
#include "unity.h"

#include "string.h"

#define CHUNK_SIZE 0xFF

void jat_reader_buffer_zero_length_input_test(void) {
    const char *test_data = "NO";

    enum jat_status status; 
    struct jat_reader_buffer reader;

    status = jat_reader_buffer_init(
        0,
        test_data,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_INVALID_ARGUMENTS, status);
}

void jat_reader_buffer_null_input_test(void) {
    enum jat_status status; 
    struct jat_reader_buffer reader;

    status = jat_reader_buffer_init(
        CHUNK_SIZE,
        NULL,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_INVALID_ARGUMENTS, status);
}

void jat_reader_buffer_valid_data_test(void) {
    const char *test_data = "Hello, World!";
    const size_t test_data_size = strnlen_s(test_data, CHUNK_SIZE);

    enum jat_status status; 
    struct jat_reader_buffer reader;

    status = jat_reader_buffer_init(
        test_data_size,
        test_data,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);

    status = jat_reader_buffer_deinit(
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
}

void jat_reader_buffer_peek_invalid_input_test(void) {
    const char *test_data = "Hello, World!";
    const size_t test_data_size = strnlen_s(test_data, CHUNK_SIZE);

    char buffer[CHUNK_SIZE];
    size_t peeked_bytes;

    enum jat_status status; 
    struct jat_reader_buffer reader;

    status = jat_reader_buffer_init(
        test_data_size,
        test_data,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);

    status = jat_reader_buffer_peek(
        &reader,
        0,
        buffer,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_INVALID_ARGUMENTS, status);

    status = jat_reader_buffer_peek(
        &reader,
        CHUNK_SIZE,
        NULL,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_INVALID_ARGUMENTS, status);

    status = jat_reader_buffer_deinit(
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
}

void jat_reader_buffer_peek_immutability_test(void) {
    const char *test_data = "Hello, World!";
    const size_t test_data_size = strnlen_s(test_data, CHUNK_SIZE);

    char buffer[CHUNK_SIZE];
    size_t peeked_bytes;

    enum jat_status status; 
    struct jat_reader_buffer reader;

    status = jat_reader_buffer_init(
        test_data_size,
        test_data,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);

    status = jat_reader_buffer_peek(
        &reader,
        CHUNK_SIZE,
        buffer,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
    TEST_ASSERT_EQUAL(test_data_size, peeked_bytes);
    TEST_ASSERT_EQUAL_MEMORY(test_data, buffer, peeked_bytes);

    status = jat_reader_buffer_peek(
        &reader,
        CHUNK_SIZE,
        buffer,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
    TEST_ASSERT_EQUAL(test_data_size, peeked_bytes);
    TEST_ASSERT_EQUAL_MEMORY(test_data, buffer, peeked_bytes);

    status = jat_reader_buffer_deinit(
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
}

void jat_reader_buffer_consume_invalid_input_test(void) {
    const char *test_data = "Hello, World!";
    const size_t test_data_size = strnlen_s(test_data, CHUNK_SIZE);

    char buffer[CHUNK_SIZE];
    size_t peeked_bytes;

    enum jat_status status; 
    struct jat_reader_buffer reader;

    status = jat_reader_buffer_init(
        test_data_size,
        test_data,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);

    status = jat_reader_buffer_consume(
        &reader,
        0,
        buffer,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_INVALID_ARGUMENTS, status);

    status = jat_reader_buffer_consume(
        &reader,
        CHUNK_SIZE,
        NULL,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_INVALID_ARGUMENTS, status);

    status = jat_reader_buffer_deinit(
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
}

void jat_reader_buffer_consume_mutability_test(void) {
    const char *test_data = "Hello, World!";
    const size_t test_data_size = strnlen_s(test_data, CHUNK_SIZE);

    char buffer[CHUNK_SIZE];
    size_t peeked_bytes;

    enum jat_status status; 
    struct jat_reader_buffer reader;

    status = jat_reader_buffer_init(
        test_data_size,
        test_data,
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);

    status = jat_reader_buffer_consume(
        &reader,
        CHUNK_SIZE,
        buffer,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
    TEST_ASSERT_EQUAL(test_data_size, peeked_bytes);
    TEST_ASSERT_EQUAL_MEMORY(test_data, buffer, peeked_bytes);

    status = jat_reader_buffer_consume(
        &reader,
        CHUNK_SIZE,
        buffer,
        &peeked_bytes
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
    TEST_ASSERT_EQUAL(0, peeked_bytes);

    status = jat_reader_buffer_deinit(
        &reader
    );

    TEST_ASSERT_EQUAL(JAT_STATUS_SUCCESS, status);
}
