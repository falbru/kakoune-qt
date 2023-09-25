#include "kakounewidget.h"
#include "rpc.h"

void KakouneWidget::handleRequest(QJsonObject request) {
    const QString method = request["method"].toString();
    QJsonArray request_params = request["params"].toArray();

    if (method == "draw") {
        RPC::DrawRequest request = RPC::deserializeDrawRequest(request_params);
        m_lines = request.lines;
        m_default_face = request.default_face;
        m_padding_face = request.padding_face;
        repaint();
    } else if(method == "draw_status") {
        RPC::DrawStatusRequest request = RPC::deserializeDrawStatusRequest(request_params);
        m_status_line = request.status_line;
        m_mode_line = request.mode_line;
        m_status_default_face = request.default_face;
        repaint();
    }else {
        qDebug() << "Unkown method: " << method;
    }
}

KakouneWidget::KakouneWidget(QWidget *parent) : QWidget(parent) {
    qDebug("Constructing kakounewidget");

    m_client = new KakouneClient();
    connect(m_client, &KakouneClient::handleRequest, this, &KakouneWidget::handleRequest);

    setFont("monospace", 11);
}

KakouneWidget::~KakouneWidget() {
    delete m_client;
}

void KakouneWidget::setFont(const QString& font_name, int font_size) {
    m_font = QFont(font_name, font_size);

    QFontMetrics font_metrics(m_font);
    QRect boundingBoxSingleChar = font_metrics.boundingRect("A");
    m_cell_size = QSize(boundingBoxSingleChar.width(), boundingBoxSingleChar.height()); // FIXME only works for monospaced fonts
}

QColor KakouneWidget::getColor(RPC::Color color, RPC::Color default_color) {
    if (color == "default") {
        color = default_color; 
    } else if (color == "black" || color == "bright-black") {
        return Qt::gray;
    } else if (color == "red" || color == "bright-red") {
        return Qt::red;
    } else if (color == "green" || color == "bright-green") {
        return Qt::green;
    } else if (color == "yellow" || color == "bright-yellow") {
        return Qt::yellow;
    } else if (color == "blue" || color == "bright-blue") {
        return Qt::blue;
    } else if (color == "magenta" || color == "bright-magenta") {
        return Qt::magenta;
    } else if (color == "cyan" || color == "bright-cyan") {
        return Qt::cyan;
    } else if (color == "white" || color == "bright-white") {
        return Qt::white;
    }

    QString hexCode = color.mid(color.indexOf(":")+1);
    return QColor("#" + hexCode);
}

void KakouneWidget::drawAtom(QPainter& painter, RPC::Atom atom, QPoint position, const RPC::Face& default_face) {
    QColor fg = getColor(atom.face.fg, default_face.fg);
    QColor bg = getColor(atom.face.bg, default_face.bg);

    int inf = 999;

    painter.setPen(fg);
    painter.fillRect(position.x(), position.y(), m_cell_size.width() * atom.contents.size(), m_cell_size.height(), bg);
    painter.drawText(QRect(position.x(), position.y(), inf, inf), Qt::AlignTop, atom.contents);
}

void KakouneWidget::drawLine(QPainter& painter, RPC::Line line, QPoint position, const RPC::Face& default_face) {
    for (RPC::Atom atom : line) {
        drawAtom(painter, atom, position, default_face);
        position.setX(position.x() + QFontMetrics(m_font).size(Qt::TextSingleLine, atom.contents).width());
    }
}

void KakouneWidget::paintEvent(QPaintEvent *) {
    qDebug("Rerender kakounewidget");

    QPainter painter(this);
    painter.setFont(m_font);
    painter.fillRect(rect(), getColor(m_default_face.bg, m_default_face.bg));

    for (int i = 0; i < m_lines.size(); ++i) {
        QPoint position(0, i * m_cell_size.height());
        drawLine(painter, m_lines[i], position, m_default_face);
    }

    // draw status
    painter.fillRect(QRect(0, rect().bottom() - m_cell_size.height(), rect().width(), m_cell_size.height()), getColor(m_status_default_face.bg, m_status_default_face.bg));
    drawLine(painter, m_status_line, QPoint(0, rect().bottom() - m_cell_size.height()), m_default_face);

    int length = 0;
    for (auto atom : m_mode_line) {
        length += atom.contents.size();
    }
    drawLine(painter, m_mode_line, QPoint(rect().width() - m_cell_size.width() * length, rect().bottom() - m_cell_size.height()), m_status_default_face);
}

QString keyCodeToString(int keyCode, Qt::KeyboardModifiers modifiers) {
    QString key;
    bool isSpecial = true;

    switch(keyCode) {
        case Qt::Key_Return:
            key = "ret";
            break;
        case Qt::Key_Space:
            key = "space";
            break;
        case Qt::Key_Tab:
            key = "tab";
            break;
        case Qt::Key_BracketLeft:
            key = "lt";
            break;
        case Qt::Key_BracketRight:
            key = "gt";
            break;
        case Qt::Key_Backspace:
            key = "backspace";
            break;
        case Qt::Key_Escape:
            key = "esc";
            break;
        case Qt::Key_Up:
            key = "up";
            break;
        case Qt::Key_Down:
            key = "down";
            break;
        case Qt::Key_Left:
            key = "left";
            break;
        case Qt::Key_Right:
            key = "right";
            break;
        case Qt::Key_PageUp:
            key = "pageup";
            break;
        case Qt::Key_PageDown:
            key = "pagedown";
            break;
        case Qt::Key_Home:
            key = "home";
            break;
        case Qt::Key_End:
            key = "end";
            break;
        case Qt::Key_Backtab:
            key = "backtab";
            break;
        case Qt::Key_Delete:
            key = "del";
            break;
        default:
            key = QKeySequence(keyCode).toString();
            isSpecial = false;
            break;
    }

    if (!isSpecial && keyCode > 0x0ff) {
        return "";
    }


    bool has_shift = modifiers.testFlag(Qt::KeyboardModifier::ShiftModifier);
    bool has_ctrl = modifiers.testFlag(Qt::KeyboardModifier::ControlModifier);
    bool has_alt = modifiers.testFlag(Qt::KeyboardModifier::AltModifier);

    if (!isSpecial && !has_shift) {
        key = key.toLower();
    }
    if (has_ctrl) {
        key = "c-" + key;
    }
    if (has_alt) {
        key = "a-" + key;
    }
    if (isSpecial || has_alt || has_ctrl) {
        key = "<" + key + ">";
    }

    return key;
}

void KakouneWidget::keyPressEvent(QKeyEvent* ev) {
    QString key = keyCodeToString(ev->key(), ev->modifiers());
    if (key == "") return;

    m_client->sendKeys(key);
}

void KakouneWidget::resizeEvent(QResizeEvent* ev) {
    qDebug() << "Resize event";
    m_client->resize(height() / m_cell_size.height() - 1, width() / m_cell_size.width());
}
