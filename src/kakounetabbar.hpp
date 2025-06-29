#ifndef KakouneTabBar_HPP
#define KakouneTabBar_HPP

#include "ipc.hpp"
#include "kakounesession.hpp"
#include <QtWidgets>
#include <qnamespace.h>

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
        void setPersistentTabs(const QList<QString> &persistent_tabs);
        void setSelectedTab(const QString &bufname);

    private:
        KakouneSession* m_session;

        const QColor m_temporary_tab_color = Qt::darkGray;
        QColor m_default_text_color;

        QList<QString> m_persistent_tabs;
        QString m_selected_tab;
};

#endif
