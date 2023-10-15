#include "kakounewidget.hpp"

KakouneWidget::KakouneWidget(const QString &session_id, QWidget *parent) : QWidget(parent)
{
    qDebug("Constructing kakounewidget");

    m_client = new KakouneClient(session_id);
    connect(m_client, &KakouneClient::refresh, this, &KakouneWidget::refresh);
    connect(m_client, &KakouneClient::finished, this, &KakouneWidget::finished);

    setFont("monospace", 11);
}

KakouneWidget::~KakouneWidget()
{
    delete m_client;
}

void KakouneWidget::refresh()
{
    repaint();
}

void KakouneWidget::setFont(const QString &font_name, int font_size)
{
    m_font = QFont(font_name, font_size);

    QFontMetrics font_metrics(m_font);
    QRect boundingBoxSingleChar = font_metrics.boundingRect("A");
    m_cell_size =
        QSize(boundingBoxSingleChar.width(), boundingBoxSingleChar.height()); // FIXME only works for monospaced fonts
}

void KakouneWidget::paintEvent(QPaintEvent *)
{
    qDebug("Rerender kakounewidget");

    QPainter painter(this);
    painter.setFont(m_font);
    painter.fillRect(rect(), m_client->getDefaultFace().getBg().toQColor(m_client->getDefaultFace().getBg()));

    DrawContext context{painter, m_cell_size};

    QList<RPC::Line> lines = m_client->getLines();
    for (int i = 0; i < lines.size(); ++i)
    {
        QPoint position(0, i * m_cell_size.height());
        lines[i].draw(context, position, m_client->getDefaultFace());
    }

    // draw status
    RPC::Face status_default_face = m_client->getDefaultFace();
    painter.fillRect(QRect(0, rect().bottom() - m_cell_size.height(), rect().width(), m_cell_size.height()),
                     status_default_face.getBg().toQColor(status_default_face.getBg()));
    m_client->getStatusLine().draw(context, QPoint(0, rect().bottom() - m_cell_size.height()), status_default_face);

    RPC::Line mode_line = m_client->getModeLine();
    int length = 0;
    for (auto atom : mode_line.getAtoms())
    {
        length += atom.getContents().size();
    }
    mode_line.draw(context,
                   QPoint(rect().width() - m_cell_size.width() * length, rect().bottom() - m_cell_size.height()),
                   status_default_face);
}

QString keyCodeToKakouneKey(int key_code, Qt::KeyboardModifiers modifiers)
{
    struct SpecialKey
    {
        QString key;
        bool is_function_key;
    };

    static QMap<int, SpecialKey> special_keys;
    if (special_keys.empty())
    {
        special_keys[Qt::Key_Less] = SpecialKey{"lt", false};
        special_keys[Qt::Key_Greater] = SpecialKey{"gt", false};
        special_keys[Qt::Key_Plus] = SpecialKey{"plus", false};
        special_keys[Qt::Key_Minus] = SpecialKey{"minus", false};
        special_keys[Qt::Key_Return] = SpecialKey{"ret", true};
        special_keys[Qt::Key_Space] = SpecialKey{"space", true};
        special_keys[Qt::Key_Tab] = SpecialKey{"tab", true};
        special_keys[Qt::Key_Backtab] = SpecialKey{"tab", true}; // Will become <s-tab>
        special_keys[Qt::Key_Backspace] = SpecialKey{"backspace", true};
        special_keys[Qt::Key_Delete] = SpecialKey{"del", true};
        special_keys[Qt::Key_Escape] = SpecialKey{"esc", true};
        special_keys[Qt::Key_Up] = SpecialKey{"up", true};
        special_keys[Qt::Key_Down] = SpecialKey{"down", true};
        special_keys[Qt::Key_Left] = SpecialKey{"left", true};
        special_keys[Qt::Key_Right] = SpecialKey{"right", true};
        special_keys[Qt::Key_PageUp] = SpecialKey{"pageup", true};
        special_keys[Qt::Key_PageDown] = SpecialKey{"pagedown", true};
        special_keys[Qt::Key_Home] = SpecialKey{"home", true};
        special_keys[Qt::Key_End] = SpecialKey{"end", true};
        special_keys[Qt::Key_Insert] = SpecialKey{"ins", true};
        special_keys[Qt::Key_Semicolon] = SpecialKey{"semicolon", false};
        special_keys[Qt::Key_Percent] = SpecialKey{"percent", false};
    }

    QString key;

    auto special_keys_it = special_keys.find(key_code);
    if (special_keys_it == special_keys.end())
    {
        key = QKeySequence(key_code).toString();
    }
    else
    {
        key = special_keys_it.value().key;
    }

    if (special_keys_it == special_keys.end() && key_code > 0x0ff)
    {
        return "";
    }

    bool has_shift = modifiers.testFlag(Qt::KeyboardModifier::ShiftModifier);
    bool has_ctrl = modifiers.testFlag(Qt::KeyboardModifier::ControlModifier);
    bool has_alt = modifiers.testFlag(Qt::KeyboardModifier::AltModifier);

    if (special_keys_it == special_keys.end() && !has_shift)
    {
        key = key.toLower();
    }
    else if (special_keys_it != special_keys.end() && has_shift && special_keys_it.value().is_function_key)
    {
        key = "s-" + key;
    }
    if (has_ctrl)
    {
        key = "c-" + key;
    }
    if (has_alt)
    {
        key = "a-" + key;
    }
    if (special_keys_it != special_keys.end() || has_alt || has_ctrl)
    {
        key = "<" + key + ">";
    }

    return key;
}

void KakouneWidget::keyPressEvent(QKeyEvent *ev)
{
    QString key = keyCodeToKakouneKey(ev->key(), ev->modifiers());
    if (key == "")
        return;

    m_client->sendKeys(key);
}

void KakouneWidget::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint localMousePosition(ev->position().x() / m_cell_size.width(), ev->position().y() / m_cell_size.height());
    m_client->sendMouseMove(localMousePosition.y(), localMousePosition.x());
}

void KakouneWidget::mousePressEvent(QMouseEvent *ev)
{
    QPoint localMousePosition(ev->position().x() / m_cell_size.width(), ev->position().y() / m_cell_size.height());

    QString button;
    switch (ev->button())
    {
    case Qt::LeftButton:
        button = "left";
        break;
    case Qt::MiddleButton:
        button = "middle";
        break;
    case Qt::RightButton:
        button = "right";
        break;
    default:
        return;
    }

    m_client->sendMousePress(button, localMousePosition.y(), localMousePosition.x());
}

void KakouneWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint localMousePosition(ev->position().x() / m_cell_size.width(), ev->position().y() / m_cell_size.height());

    QString button;
    switch (ev->button())
    {
    case Qt::LeftButton:
        button = "left";
        break;
    case Qt::MiddleButton:
        button = "middle";
        break;
    case Qt::RightButton:
        button = "right";
        break;
    default:
        return;
    }

    m_client->sendMouseRelease(button, localMousePosition.y(), localMousePosition.x());
}

void KakouneWidget::resizeEvent(QResizeEvent *ev)
{
    qDebug() << "Resize event";
    m_client->resize(height() / m_cell_size.height() - 1, width() / m_cell_size.width());
}
