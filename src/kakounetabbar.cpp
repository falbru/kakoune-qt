#include "kakounetabbar.hpp"

KakouneTabBar::KakouneTabBar(KakouneSession* session, QWidget *parent) : QTabBar(parent), m_session(session) {
    setFocusPolicy(Qt::NoFocus);
    setTabsClosable(false);
    setAutoHide(false);
    setDocumentMode(true);
    setExpanding(false);
}

KakouneTabBar::~KakouneTabBar() {

}

void KakouneTabBar::bind(KakouneIPC::IPCServer* server) {
    connect(server, &KakouneIPC::IPCServer::setTabs, this, &KakouneTabBar::setPersistentTabs);
    connect(server, &KakouneIPC::IPCServer::setSelectedTab, this, &KakouneTabBar::setSelectedTab);
}

void KakouneTabBar::setPersistentTabs(const QList<QString> &persistent_tabs) {
    m_persistent_tabs = persistent_tabs;

    bool is_selected_tab_persistent = false;

    for (int i = 0; i < persistent_tabs.size(); ++i) {
        if (i < count()) {
            setTabText(i, persistent_tabs[i]);
            setTabTextColor(i, m_default_text_color);
        } else {
            addTab(persistent_tabs[i]);
            m_default_text_color = tabTextColor(i);
        }

        if (persistent_tabs[i] == m_selected_tab) {
            is_selected_tab_persistent = true;
        }
    }

    int tabCount = persistent_tabs.size();
    if (!is_selected_tab_persistent) tabCount++;

    while (count() > tabCount) {
        removeTab(count() - 1);
    }

}

void KakouneTabBar::setSelectedTab(const QString &bufname) {
    m_selected_tab = bufname;

    for (int i = 0; i < m_persistent_tabs.size(); i++) {
        if (m_persistent_tabs[i] == bufname) {
            setCurrentIndex(i);

            while (count() > m_persistent_tabs.size()) {
                removeTab(count() - 1);
            }

            return;
        }

    }

    int temporary_tab_index = count()-1;
    if (count() == m_persistent_tabs.size()) {
        temporary_tab_index = addTab(bufname);
    }else {
        setTabText(temporary_tab_index, bufname);
    }

    setTabTextColor(temporary_tab_index, m_temporary_tab_color);
    setCurrentIndex(temporary_tab_index);
}
