#ifndef KakouneTabBar_HPP
#define KakouneTabBar_HPP

#include "ipc.hpp"
#include "kakounesession.hpp"
#include <QtWidgets>

class KakouneTabBar : public QTabBar
{
    Q_OBJECT

    public:
        KakouneTabBar(KakouneSession* session, QWidget *parent = nullptr);
        ~KakouneTabBar();

        void bind(KakouneIPC::IPCServer* server);

    signals:
        void selectTab(const QString& selectedTab);

    public slots:
        void setTabs(const QList<QString> &tabs);
        void setSelectedTab(const QString &bufname);

    private:
        KakouneSession* m_session;
        QColor m_default_text_color;

        QList<QString> m_tabs;
        QString m_selected_tab;
};

#endif
