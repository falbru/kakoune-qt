#include "kakounemenu.hpp"
#include "rpc/line.hpp"

KakouneMenu::KakouneMenu(KakouneClient *client, DrawOptions *draw_options, QWidget *parent) : QWidget(parent) {
    visible = false;

    m_client = client;
    m_draw_options = draw_options;

    resize(1000, 1000);
}

KakouneMenu::~KakouneMenu() {

}

void KakouneMenu::paintEvent(QPaintEvent *ev) {
    // if (!visible) {
    //  return;
    // }

    QPainter painter(this);
    painter.setFont(m_draw_options->getFont());

    DrawContext context{painter, m_draw_options->getCellSize()};

    QList<RPC::Line> items = m_client->getMenuItems();
    for (int i = 0; i < items.size(); ++i)
    {
        QPoint position(0, i * m_draw_options->getCellSize().height());
        items[i].draw(context, position, m_client->getMenuFace());
    }
}
