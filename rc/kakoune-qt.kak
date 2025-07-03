provide-module kakoune-qt %{
declare-option -hidden str kakqt_focused_client

hook global FocusIn .* %{
    set-option global kakqt_focused_client %val{client}
}

hook global SessionRenamed .*:.* %{
    evaluate-commands %sh{
        for client in $kak_client_list; do
            printf 'evaluate-commands -client "%s" %%{ nop %%sh{ KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt --cli rename-session %s }}\n' $client $kak_session
        done
    }
}

hook global WinDisplay .* %{
    nop %sh{
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli set-selected-tab $kak_bufname
    }
}

hook global FocusIn .* %{
    nop %sh{
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli set-selected-tab $kak_bufname
    }
}

hook global GlobalSetOption harpoon_files=.* %{
    evaluate-commands %sh{
        for client in $kak_client_list; do
            printf 'evaluate-commands -client "%s" %%{ nop %%sh{ KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt --cli set-tabs $kak_opt_harpoon_files }}\n' $client
        done
        printf 'evaluate-commands -client "%s" %%{ nop %%sh{ KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID kak-qt --cli set-selected-tab $kak_bufname }}\n' $kak_opt_kakqt_focused_client
    }
}

hook global ClientCreate .* %{
    nop %sh{
        KAKQT_CLIENT_ID=$kak_client_env_KAKQT_CLIENT_ID \
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli rename-client $kak_client
    }
}

hook global ClientRenamed .*:.* %{
    nop %sh{
        KAKQT_CLIENT_ID=$kak_client_env_KAKQT_CLIENT_ID \
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli rename-client $kak_client
    }
}

define-command kakqt-split-horizontal -params .. -docstring "kakqt-split-horizontal [<commands>]: create a new Kakoune client" %{
    nop %sh{
        KAKQT_CLIENT_ID=$kak_client_env_KAKQT_CLIENT_ID \
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli split-horizontal $@
    }
}
complete-command -menu kakqt-split-horizontal command

define-command kakqt-split-vertical -params .. -docstring "kakqt-split-vertical [<commands>]: create a new Kakoune client" %{
    nop %sh{
        KAKQT_CLIENT_ID=$kak_client_env_KAKQT_CLIENT_ID \
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli split-vertical $@
    }
}
complete-command -menu kakqt-split-vertical command

alias global new kakqt-split-horizontal

define-command kakqt-focus -params 1 -docstring '
kakqt-focus [<client>]: focus the given client' %{
    nop %sh{
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli focus $1
    }
}
complete-command -menu kakqt-focus client

define-command kakqt-show -params 1 -docstring '
kakqt-show [<client>]: show the given client
' \
%{
    nop %sh{
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli show $1
    }
}
complete-command -menu kakqt-show client

define-command kakqt-hide -params 1 -docstring '
kakqt-hide [<client>]: hide the given client
' \
%{
    nop %sh{
        KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
        kak-qt --cli hide $1
    }
}
complete-command -menu kakqt-hide client

}

hook global ClientCreate .* %{
    evaluate-commands %sh{
        if ! [ -z "${kak_client_env_KAKQT_SESSION_ID}" ]; then
            echo "require-module kakoune-qt"

            KAKQT_SESSION_ID=$kak_client_env_KAKQT_SESSION_ID \
            KAKQT_CLIENT_ID=$kak_client_env_KAKQT_CLIENT_ID \
            kak-qt --cli rename-client $kak_client
        fi
    }
}
