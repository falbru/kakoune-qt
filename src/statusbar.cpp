#include "statusbar.hpp"

StatusBar::StatusBar(DrawOptions *draw_options, QWidget *parent)
{
    m_draw_options = draw_options;
    connect(draw_options, &DrawOptions::updated, this, &StatusBar::drawOptionsUpdated);
    drawOptionsUpdated();
}

StatusBar::~StatusBar()
{
}

void StatusBar::setActiveClient(KakouneClient *client)
{
    m_client = client;
}

void StatusBar::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    painter.setFont(m_draw_options->getFont());

    DrawContext context{painter, m_draw_options->getColorPalette(), m_draw_options->getCellSize()};

    RPC::Face status_default_face = m_client->getStatusDefaultFace();
    painter.fillRect(
        rect(), status_default_face.getBg().toQColor(status_default_face.getBg(), m_draw_options->getColorPalette()));

    RPC::Line mode_line = m_client->getModeLine();
    int length = 0;
    for (auto atom : mode_line.getAtoms())
    {
        length += atom.getContents().size();
    }
    mode_line.draw(context, QPoint(rect().width() - m_draw_options->getCellSize().width() * length, 0),
                   status_default_face);

    m_client->getStatusLine().draw(context, QPoint(), status_default_face);
}

void StatusBar::drawOptionsUpdated()
{
    setFixedHeight(m_draw_options->getCellSize().height());
}
