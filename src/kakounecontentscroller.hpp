#ifndef KAKOUNECONTENTSCROLLER
#define KAKOUNECONTENTSCROLLER

#include "kakounecontent.hpp"
#include <QtWidgets>

class KakouneContentScroller : public QWidget
{
    Q_OBJECT

    public:
        KakouneContentScroller(KakouneContent* kakoune_content, QWidget *parent = nullptr);
        ~KakouneContentScroller();

        void setContent(QList<RPC::Line> content);
        const QList<RPC::Line> &getContent() const;

        void setDefaultFace(RPC::Face default_face);
        const RPC::Face &getDefaultFace() const;

    private:
        KakouneContent* m_kakoune_content;
        QScrollBar* m_scroll_bar;
};

#endif
