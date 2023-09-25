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
    static QMap<int, QString> special_keys;
    if (special_keys.empty()) {
        special_keys[Qt::Key_Return] = "ret";
        special_keys[Qt::Key_Space] = "space";
        special_keys[Qt::Key_Tab] = "tab";
        special_keys[Qt::Key_BracketLeft] = "lt";
        special_keys[Qt::Key_BracketRight] = "gt";
        special_keys[Qt::Key_Backspace] = "backspace";
        special_keys[Qt::Key_Escape] = "esc";
        special_keys[Qt::Key_Up] = "up";
        special_keys[Qt::Key_Down] = "down";
        special_keys[Qt::Key_Left] = "left";
        special_keys[Qt::Key_Right] = "right";
        special_keys[Qt::Key_PageUp] = "pageup";
        special_keys[Qt::Key_PageDown] = "pagedown";
        special_keys[Qt::Key_Home] = "home";
        special_keys[Qt::Key_End] = "end";
        special_keys[Qt::Key_Backtab] = "backtab";
        special_keys[Qt::Key_Delete] = "del";
    }

    QString key;

    auto special_keys_it = special_keys.find(keyCode);
    if (special_keys_it == special_keys.end()) {
        key = QKeySequence(keyCode).toString();
    }else {
        key = special_keys_it.value();
    }

    if (special_keys_it == special_keys.end() && keyCode > 0x0ff) {
        return "";
    }

    bool has_shift = modifiers.testFlag(Qt::KeyboardModifier::ShiftModifier);
    bool has_ctrl = modifiers.testFlag(Qt::KeyboardModifier::ControlModifier);
    bool has_alt = modifiers.testFlag(Qt::KeyboardModifier::AltModifier);

    if (special_keys_it == special_keys.end() && !has_shift) {
        key = key.toLower();
    }
    if (has_ctrl) {
        key = "c-" + key;
    }
    if (has_alt) {
        key = "a-" + key;
    }
    if (special_keys_it != special_keys.end() || has_alt || has_ctrl) {
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
