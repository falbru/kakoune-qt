#include "kakounetabbar.hpp"

KakouneTabBar::KakouneTabBar(KakouneSession *session, QWidget *parent) : QTabBar(parent), m_session(session)
{
    setFocusPolicy(Qt::NoFocus);
    setTabsClosable(false);
    setAutoHide(false);
    setDocumentMode(true);
    setExpanding(false);
}

KakouneTabBar::~KakouneTabBar()
{
}

void KakouneTabBar::bind(KakouneIPC::IPCServer *server)
{
    connect(server, &KakouneIPC::IPCServer::setTabs, this, &KakouneTabBar::setPersistentTabs);
    connect(server, &KakouneIPC::IPCServer::setSelectedTab, this, &KakouneTabBar::setSelectedTab);
}

QString KakouneTabBar::getBufnameAtIndex(int index)
{
    return m_persistent_tabs[index];
}

void KakouneTabBar::setPersistentTabs(const QList<QString> &persistent_tabs)
{
    m_persistent_tabs = persistent_tabs;

    updateTabs();
}

void KakouneTabBar::setSelectedTab(const QString &bufname)
{
    m_selected_tab = bufname;

    updateTabs();
}

QStringList getUniqueBasenames(const QStringList &bufnames)
{
    QStringList result;
    QMap<QString, QStringList> path_map;

    for (const QString &path : bufnames)
    {
        QStringList components = path.split('/', Qt::SkipEmptyParts);
        path_map[path] = components;
    }

    for (const QString &path : bufnames)
    {
        const QStringList &components = path_map[path];
        if (components.isEmpty())
        {
            result.append("");
            continue;
        }

        int suffix_length = 1;
        QString candidate;

        while (suffix_length <= components.size())
        {
            QStringList suffix = components.mid(components.size() - suffix_length, suffix_length);
            candidate = suffix.join('/');

            bool is_unique = true;
            for (const QString &other_path : bufnames)
            {
                if (other_path == path)
                    continue;

                const QStringList &other_components = path_map[other_path];
                if (other_components.size() >= suffix_length)
                {
                    QStringList otherSuffix =
                        other_components.mid(other_components.size() - suffix_length, suffix_length);
                    QString other_candidate = otherSuffix.join('/');

                    if (candidate == other_candidate)
                    {
                        is_unique = false;
                        break;
                    }
                }
            }

            if (is_unique)
            {
                break;
            }

            suffix_length++;
        }

        result.append(candidate);
    }

    return result;
}

void KakouneTabBar::updateTabs()
{
    bool is_selected_tab_persistent = false;
    int selected_tab_index = -1;

    QList<QString> tabs(m_persistent_tabs.size());

    for (int i = 0; i < m_persistent_tabs.size(); i++)
    {
        tabs[i] = m_persistent_tabs[i];

        if (m_persistent_tabs[i] == m_selected_tab)
        {
            is_selected_tab_persistent = true;
            selected_tab_index = i;
        }
    }
    if (!is_selected_tab_persistent)
    {
        tabs.push_back(m_selected_tab);
        selected_tab_index = tabs.size() - 1;
    }

    QList<QString> tabs_display_text = getUniqueBasenames(tabs);

    for (int i = 0; i < tabs_display_text.size(); ++i)
    {
        if (i < count())
        {
            setTabText(i, tabs_display_text[i]);
            if (i < m_persistent_tabs.size())
            {
                setTabTextColor(i, m_default_text_color);
            }
        }
        else
        {
            addTab(tabs_display_text[i]);
            m_default_text_color = tabTextColor(i);
        }
    }

    while (count() > tabs_display_text.size())
    {
        removeTab(count() - 1);
    }

    setCurrentIndex(selected_tab_index);
    if (!is_selected_tab_persistent)
    {
        setTabTextColor(selected_tab_index, m_temporary_tab_color);
    }
}
