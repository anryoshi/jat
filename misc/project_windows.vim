let g:envs = {
  \ "msvc": {
    \ "build_dir": ".build_msvc",
    \ "cc": "cl"
  \ },
  \ "clang": {
    \ "build_dir": ".build_clang",
    \ "cc": "clang-cl"
  \ }
\ }

let g:default_env = "msvc"

function! g:SetupEnv(env_name) abort
    let current_env = g:envs[a:env_name]
    let g:meson_build_dir = current_env.build_dir
    MesonInit
endfunction

call g:SetupEnv(g:default_env)

command! -nargs=1 SwitchEnv :call g:SetupEnv(<q-args>)
