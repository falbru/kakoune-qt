#include "kakounewidget.hpp"
#include "colorpalette.hpp"
#include "rpc/color.hpp"
#include "statusbar.hpp"

KakouneWidget::KakouneWidget(const QString &session_id, const QUuid &window_id, DrawOptions *draw_options,
                             QWidget *parent)
    : KakouneWidget(session_id, window_id, draw_options, "", parent)
{
}

KakouneWidget::KakouneWidget(const QString &session_id, const QUuid &window_id, DrawOptions *draw_options,
                             QString client_arguments, QWidget *parent)
    : QWidget(parent)
{
    m_id = QUuid::createUuid();
    m_draw_options = draw_options;

    m_client = new KakouneClient(session_id, client_arguments,
                                 {{"KAKQT_SESSION_ID", window_id.toString()}, {"KAKQT_WINDOW_ID", m_id.toString()}});

    connect(m_client, &KakouneClient::refresh, this, &KakouneWidget::clientRefreshed);
    connect(m_client, &KakouneClient::finished, this, &KakouneWidget::finished);
    connect(m_client, &KakouneClient::setUIOptions, this, &KakouneWidget::setUIOptions);

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

void KakouneWidget::installEventFilter(QObject *filter)
{
    m_textedit->installEventFilter(filter);
}

void KakouneWidget::setUIOptions(QMap<QString, QString> options)
{
    for (auto option = options.begin(); option != options.end(); option++)
    {
        if (option.key() == "gui_set_font")
        {
            int lastSpaceIndex = option.value().lastIndexOf(' ');

            QString font_name = option.value().left(lastSpaceIndex);
            QString font_size_str = option.value().mid(lastSpaceIndex + 1);

            bool ok;
            int font_size = font_size_str.toInt(&ok);
            if (!ok)
            {
                qDebug() << "Error converting font size to integer.";
                continue;
            }

            m_draw_options->setFont(font_name, font_size);
        }
        else if (option.key().length() > 13 && option.key().mid(0, 13) == "gui_set_color")
        {
            auto setColor = [&](auto setColorFunc) {
                ColorPalette new_color_palette = m_draw_options->getColorPalette();
                (new_color_palette.*
                 setColorFunc)(RPC::Color(option.value()).toQColor(m_draw_options->getColorPalette()));
                m_draw_options->setColorPalette(new_color_palette);
            };

            if (option.key() == "gui_set_color_gray")
            {
                setColor(&ColorPalette::setGray);
            }
            else if (option.key() == "gui_set_color_red")
            {
                setColor(&ColorPalette::setRed);
            }
            else if (option.key() == "gui_set_color_green")
            {
                setColor(&ColorPalette::setGreen);
            }
            else if (option.key() == "gui_set_color_yellow")
            {
                setColor(&ColorPalette::setYellow);
            }
            else if (option.key() == "gui_set_color_blue")
            {
                setColor(&ColorPalette::setBlue);
            }
            else if (option.key() == "gui_set_color_magenta")
            {
                setColor(&ColorPalette::setMagenta);
            }
            else if (option.key() == "gui_set_color_cyan")
            {
                setColor(&ColorPalette::setCyan);
            }
            else if (option.key() == "gui_set_color_white")
            {
                setColor(&ColorPalette::setWhite);
            }
        }
        else
        {
            qDebug() << "Unknown ui option: " << option.key();
        }
    }
}

KakouneWidget *KakouneWidget::findParentKakouneWidget(QWidget *widget)
{
    QWidget *parent_kak_widget = widget->parentWidget();
    while (parent_kak_widget)
    {
        KakouneWidget *kak_widget = qobject_cast<KakouneWidget *>(parent_kak_widget);
        if (kak_widget)
        {
            return kak_widget;
        }
        parent_kak_widget = parent_kak_widget->parentWidget();
    }
    return nullptr;
}

void KakouneWidget::hideEvent(QHideEvent *event)
{
    emit hidden();
}
