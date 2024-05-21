provide-module kakoune-qt %{

hook global SessionRenamed .*:.* %{
    nop %sh{ KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt cli rename-session $kak_session }
}

define-command -override -docstring "new [<commands>]: create a new Kakoune client" new -params .. %{ nop %sh{
    KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt cli new-client $@
}}
complete-command -menu new command

define-command kakqt-focus -params ..1 -docstring '
kakqt-focus [<client>]: focus the given client
If no client is passed then the current one is used' \
%{
    evaluate-commands %sh{
        if [ $# -eq 1 ]; then
            printf "evaluate-commands -client '%s' kakqt-focus" "$1"
        elif [ -n "$kak_client_env_KAKQT_WINDOW_ID" ]; then
            KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt cli focus $kak_client_env_KAKQT_WINDOW_ID
        fi
    }
}
complete-command -menu kakqt-focus client

}
