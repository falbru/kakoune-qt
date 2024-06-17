provide-module kakoune-qt %{

hook global SessionRenamed .*:.* %{
    nop %sh{ KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt cli rename-session $kak_session }
}

define-command kakqt-split-horizontal -params .. -docstring "kakqt-split-horizontal [<commands>]: create a new Kakoune client" %{ nop %sh{
    KAKQT_WINDOW_ID=$kak_client_env_KAKQT_WINDOW_ID KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt cli split-horizontal $@
}}
complete-command -menu kakqt-split-horizontal command

define-command kakqt-split-vertical -params .. -docstring "kakqt-split-vertical [<commands>]: create a new Kakoune client" %{ nop %sh{
    KAKQT_WINDOW_ID=$kak_client_env_KAKQT_WINDOW_ID KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt cli split-vertical $@
}}
complete-command -menu kakqt-split-vertical command

alias global new kakqt-split-horizontal

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
