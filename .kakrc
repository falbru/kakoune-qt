hook global BufSetOption cpp %{
    set-option buffer formatcmd 'clang-format --style Microsoft'
}
