#include "kakounewidget.hpp"
#include "statusbar.hpp"

KakouneWidget::KakouneWidget(const QString &session_id, DrawOptions *draw_options, QWidget *parent)
    : KakouneWidget(session_id, draw_options, "", parent)
{
}

KakouneWidget::KakouneWidget(const QString &session_id, DrawOptions *draw_options, QString client_arguments,
                             QWidget *parent)
    : QWidget(parent)
{
    m_id = QUuid::createUuid();
    m_draw_options = draw_options;

    m_client = new KakouneClient(session_id, client_arguments, {{"KAKQT_WINDOW_ID", m_id.toString()}});
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

    setFocusProxy(m_textedit);
    this->setLayout(layout);
}

QSize KakouneWidget::sizeHint() const
{
    return QSize(512, 256);
}

KakouneWidget::~KakouneWidget()
{
    delete m_client;
}

QUuid KakouneWidget::getID()
{
    return m_id;
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
