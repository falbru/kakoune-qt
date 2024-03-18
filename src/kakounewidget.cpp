#include "kakounewidget.hpp"
#include "statusbar.hpp"

KakouneWidget::KakouneWidget(const QString &session_id, DrawOptions *draw_options, QWidget *parent) : QWidget(parent)
{
    qDebug("Constructing kakounewidget");

    m_draw_options = draw_options;

    m_client = new KakouneClient(session_id);
    connect(m_client, &KakouneClient::refresh, this, &KakouneWidget::clientRefreshed);
    connect(m_client, &KakouneClient::finished, this, &KakouneWidget::finished);

    m_textedit = new KakouneTextEdit(m_client, draw_options, this);
    m_menu = new KakouneMenu(m_client, draw_options, m_textedit);
    m_info_box = new KakouneInfoBox(m_client, m_menu, draw_options, m_textedit);

    StatusBar *status_bar = new StatusBar(draw_options);
    connect(this, &KakouneWidget::refresh, this, [=]() { status_bar->repaint(); });
    status_bar->setActiveClient(getClient());

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_textedit);
    layout->addWidget(status_bar);

    this->setLayout(layout);
}

KakouneWidget::~KakouneWidget()
{
    delete m_client;
}

KakouneClient *KakouneWidget::getClient()
{
    return m_client;
}

void KakouneWidget::clientRefreshed()
{
    repaint();
    emit refresh();
}
