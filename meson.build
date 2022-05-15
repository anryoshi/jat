project(
  'jat',
  'c',
  version: '0.1',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'werror=true',
    'default_library=static',
    'c_std=c17'
  ]
)

add_global_arguments('-D_CRT_SECURE_NO_WARNINGS', language : 'c')

unity_sp = subproject(
  'unity',
)

unity_dep = unity_sp.get_variable('unity_dep')

subdir('jat')
