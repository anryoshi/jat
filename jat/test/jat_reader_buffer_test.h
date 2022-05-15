#ifndef JAT_READER_BUFFER_TEST_H
#define JAT_READER_BUFFER_TEST_H

void jat_reader_buffer_zero_length_input_test(void);
void jat_reader_buffer_null_input_test(void);
void jat_reader_buffer_valid_data_test(void);
void jat_reader_buffer_peek_invalid_input_test(void);
void jat_reader_buffer_peek_immutability_test(void);
void jat_reader_buffer_consume_invalid_input_test(void);
void jat_reader_buffer_consume_mutability_test(void);

#endif // JAT_READER_BUFFER_TEST_H
