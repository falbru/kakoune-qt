#include "kakounemenu.hpp"
#include "rpc/line.hpp"

KakouneMenu::KakouneMenu(KakouneClient *client, DrawOptions *draw_options, QWidget *parent) : QWidget(parent), menu_height(5) {
    m_visible = false;
    m_selected_item = -1;

    m_client = client;
    m_draw_options = draw_options;

    connect(m_client, &KakouneClient::showMenu, this, &KakouneMenu::showMenu);
    connect(m_client, &KakouneClient::hideMenu, this, &KakouneMenu::hide);
    connect(m_client, &KakouneClient::selectMenuItem, this, &KakouneMenu::selectItem);

    resize(1000, 1000);
}

KakouneMenu::~KakouneMenu() {

}

void KakouneMenu::showMenu() {
    show();

    RPC::Coord anchor = m_client->getMenuAnchor();
    move(anchor.column * m_draw_options->getCellSize().width(), (anchor.line+1) * m_draw_options->getCellSize().height());

    QList<RPC::Line> items = m_client->getMenuItems();

    int max_item_length = 0;
    for (int i = 0; i < items.length(); i++) {
      QList<RPC::Atom> atoms = items[i].getAtoms();

      int item_length = 0;
      for (int j = 0; j < atoms.length(); j++) {
          item_length += atoms[j].getContents().length();
      }

      max_item_length = qMax(max_item_length, item_length);
    }

    resize(max_item_length * m_draw_options->getCellSize().width(), qMin(menu_height, items.length()) * m_draw_options->getCellSize().height());
}

void KakouneMenu::selectItem(int index) {
    if (index >= m_client->getMenuItems().count()) {
      m_selected_item = -1;
    }else {
      m_selected_item = index;
    }
}

void KakouneMenu::paintEvent(QPaintEvent *ev) {
    QPainter painter(this);
    painter.setFont(m_draw_options->getFont());

    DrawContext context{painter, m_draw_options->getCellSize()};

    painter.fillRect(0, 0, width(), height(), m_client->getMenuFace().getBg().toQColor());

    QList<RPC::Line> items = m_client->getMenuItems();

    int scrolling_index_offset = (m_selected_item / menu_height) * menu_height;
    for (int i = 0; i < items.size() - scrolling_index_offset && i < menu_height; ++i)
    {
        int index = scrolling_index_offset + i;
        QPoint position(0, i * m_draw_options->getCellSize().height());
        items[index].draw(context, position, m_selected_item == index ? m_client->getSelectedItemFace() : m_client->getMenuFace());
    }
}
