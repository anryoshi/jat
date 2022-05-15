jat_test_sources = [
  'jat_test.c',
  'jat_decoder_test.c',
  'jat_reader_buffer_test.c'
]

jat_test = executable(
  'jat_test',
  jat_test_sources,
  include_directories: [
    jat_inc,
    jat_private_inc
  ],
  link_with: [jat],
  dependencies: [unity_dep]
)

test('jat_test', jat_test)
