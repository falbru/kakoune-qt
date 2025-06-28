#include "kakounetabbar.hpp"

KakouneTabBar::KakouneTabBar(KakouneSession* session, QWidget *parent) : QTabBar(parent), m_session(session) {
    setFocusPolicy(Qt::NoFocus);
    setTabsClosable(false);
}

KakouneTabBar::~KakouneTabBar() {

}

void KakouneTabBar::bind(KakouneIPC::IPCServer* server) {
    connect(server, &KakouneIPC::IPCServer::setTabs, this, &KakouneTabBar::setTabs);
    connect(server, &KakouneIPC::IPCServer::setSelectedTab, this, &KakouneTabBar::setSelectedTab);
}

void KakouneTabBar::setTabs(const QList<QString> &tabs) {
    m_tabs = tabs;

    bool selected_in_tabs = false;
    for (int i = 0; i < tabs.size(); ++i) {
        if (i < count()) {
            setTabText(i, tabs[i]);
            setTabTextColor(i, m_default_text_color);
        } else {
            addTab(tabs[i]);
            m_default_text_color = tabTextColor(i);
        }

        if (tabs[i] == m_selected_tab) {
            selected_in_tabs = true;
        }
    }

    while (count() > tabs.size() + !selected_in_tabs ? 1 : 0) {
        removeTab(count() - 1);
    }

}

void KakouneTabBar::setSelectedTab(const QString &bufname) {
    qDebug() << "SELECT:" << bufname;
    m_selected_tab = bufname;

    for (int i = 0; i < count(); i++) {
        if (tabText(i) == bufname) {
            setCurrentIndex(i);

            while (count() > m_tabs.size()) {
                removeTab(count() - 1);
            }
            return;
        }

    }

    if (count() == m_tabs.size()) {
        addTab(bufname);
    }else {
        setTabText(count()-1, bufname);
    }
    setTabTextColor(count()-1, Qt::darkGray);
    setCurrentIndex(count()-1);
}
