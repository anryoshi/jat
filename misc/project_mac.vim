let g:envs = {
  \ "native": {
    \ "build_dir": ".build_native",
    \ "cc": "cc"
  \ },
  \ "clang": {
    \ "build_dir": ".build_clang",
    \ "cc": "clang"
  \ }
\ }

let g:default_env = "clang"

function! g:SetupEnv(env_name) abort
    let current_env = g:envs[a:env_name]
    let g:meson_build_dir = current_env.build_dir
    MesonInit
endfunction

call g:SetupEnv(g:default_env)

command! -nargs=1 SwitchEnv :call g:SetupEnv(<q-args>)
