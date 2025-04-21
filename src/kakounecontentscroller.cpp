#include "kakounecontentscroller.hpp"
#include <qscrollbar.h>

KakouneContentScroller::KakouneContentScroller(KakouneContent* kakoune_content, QWidget *parent) : QWidget(parent), m_kakoune_content(kakoune_content)
{
    m_scroll_bar = new QScrollBar();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(kakoune_content);
    layout->addWidget(m_scroll_bar);
}

KakouneContentScroller::~KakouneContentScroller() {

}
