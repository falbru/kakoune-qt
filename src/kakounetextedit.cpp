#include "kakounetextedit.hpp"
#include <qevent.h>

bool TabIgnoreFilter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Tab || keyEvent->key() == Qt::Key_Backtab)
        {
            KakouneTextEdit *textedit = dynamic_cast<KakouneTextEdit *>(qApp->focusWidget());
            if (textedit)
            {
                textedit->pressKey(keyEvent->key(), keyEvent->modifiers());
            }
            return true;
        }
    }
    return false;
}

KakouneTextEdit::KakouneTextEdit(KakouneClient *client, DrawOptions *draw_options, QWidget *parent) : QWidget(parent)
{
    m_client = client;
    m_draw_options = draw_options;

    installEventFilter(new TabIgnoreFilter());

    setFocusPolicy(Qt::StrongFocus);
}

KakouneTextEdit::~KakouneTextEdit()
{
}

void KakouneTextEdit::setActiveClient(KakouneClient *client)
{
    m_client = client;
}

void KakouneTextEdit::paintEvent(QPaintEvent *)
{
    qDebug("Rerender kakounewidget");

    QPainter painter(this);
    DrawContext context{painter, m_draw_options->getColorPalette(), m_draw_options->getCellSize()};

    painter.setFont(m_draw_options->getFont());
    painter.fillRect(rect(), m_client->getDefaultFace().getBgAsQColor(context.color_palette));

    QList<RPC::Line> lines = m_client->getLines();
    for (int i = 0; i < lines.size(); ++i)
    {
        QPoint position(0, i * m_draw_options->getCellSize().height());
        lines[i].draw(context, position, m_client->getDefaultFace());
    }
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

void KakouneTextEdit::pressKey(int key_code, Qt::KeyboardModifiers modifiers)
{
    QString key = keyCodeToKakouneKey(key_code, modifiers);
    if (key == "")
        return;

    m_client->sendKeys(key);
}

void KakouneTextEdit::keyPressEvent(QKeyEvent *ev)
{
    pressKey(ev->key(), ev->modifiers());
}

void KakouneTextEdit::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint localMousePosition(ev->position().x() / m_draw_options->getCellSize().width(),
                              ev->position().y() / m_draw_options->getCellSize().height());
    m_client->sendMouseMove(localMousePosition.y(), localMousePosition.x());
}

void KakouneTextEdit::mousePressEvent(QMouseEvent *ev)
{
    QPoint localMousePosition(ev->position().x() / m_draw_options->getCellSize().width(),
                              ev->position().y() / m_draw_options->getCellSize().height());

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

void KakouneTextEdit::mouseReleaseEvent(QMouseEvent *ev)
{
    QPoint localMousePosition(ev->position().x() / m_draw_options->getCellSize().width(),
                              ev->position().y() / m_draw_options->getCellSize().height());

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

void KakouneTextEdit::resizeEvent(QResizeEvent *ev)
{
    qDebug() << "Resize event";
    m_client->resize(height() / m_draw_options->getCellSize().height(),
                     width() / m_draw_options->getCellSize().width());
}

void KakouneTextEdit::focusInEvent(QFocusEvent *ev)
{
    m_client->sendKeys("<focus_in>");
}

void KakouneTextEdit::focusOutEvent(QFocusEvent *ev)
{
    m_client->sendKeys("<focus_out>");
}

void KakouneTextEdit::wheelEvent(QWheelEvent *ev)
{
    static int scroll = 0;
    static constexpr int scroll_sensitivity = 120;
    static constexpr int scroll_threshold = 7;

    const QPoint num_degrees = ev->angleDelta() / 8;

    if (!num_degrees.isNull())
    {
        const int scroll_amount = num_degrees.y() * 120 / scroll_sensitivity;
        scroll += scroll_amount;

        if (std::abs(scroll) >= scroll_threshold)
        {
            m_client->sendScroll(-scroll / scroll_threshold);
            scroll %= scroll_threshold;
        }
    }
    else
    {
        scroll = 0;
    }
}
