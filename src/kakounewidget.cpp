#include "kakounewidget.h"

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

void KakouneWidget::paintEvent(QPaintEvent *) {
    qDebug("Rerender kakounewidget");

    QPainter painter(this);
    painter.setFont(m_font);
    painter.fillRect(rect(), m_default_face.getBg().toQColor(m_default_face.getBg()));

    DrawContext context{painter, m_cell_size};

    for (int i = 0; i < m_lines.size(); ++i) {
        QPoint position(0, i * m_cell_size.height());
        m_lines[i].draw(context, position, m_default_face);
    }

    // draw status
    painter.fillRect(QRect(0, rect().bottom() - m_cell_size.height(), rect().width(), m_cell_size.height()), m_status_default_face.getBg().toQColor(m_status_default_face.getBg()));
    m_status_line.draw(context, QPoint(0, rect().bottom() - m_cell_size.height()), m_status_default_face);

    int length = 0;
    for (auto atom : m_mode_line.getAtoms()) {
        length += atom.getContents().size();
    }
    m_mode_line.draw(context, QPoint(rect().width() - m_cell_size.width() * length, rect().bottom() - m_cell_size.height()), m_status_default_face);
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
