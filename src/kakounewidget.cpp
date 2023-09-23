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
    }else {
        qDebug() << "Unkown method: " << method;
    }
}

KakouneWidget::KakouneWidget(QWidget *parent) : QWidget(parent) {
    qDebug("Constructing kakounewidget");

    m_client = new KakouneClient();
    connect(m_client, &KakouneClient::handleRequest, this, &KakouneWidget::handleRequest);

    setFont("monospace", 12);
}

KakouneWidget::~KakouneWidget() {
    delete m_client;
}

void KakouneWidget::setFont(const QString& font_name, int font_size) {
    m_font = QFont(font_name, font_size);

    QFontMetrics font_metrics(m_font);
    m_cell_size = QSize(font_metrics.maxWidth(), font_metrics.height()); // FIXME only works for monospaced fonts
}

QColor KakouneWidget::getColor(RPC::Color color, bool bg) {
    if (color == "default") {
        color = bg ? m_default_face.bg : m_default_face.fg; 
    }

    // TODO: add support for color literals like black, red etc.

    QString hexCode = color.mid(color.indexOf(":")+1);
    return QColor("#" + hexCode);
}

void KakouneWidget::drawAtom(QPainter& painter, RPC::Atom atom, QPoint position) {
    QColor fg = getColor(atom.face.fg, false);
    QColor bg = getColor(atom.face.bg, true);

    int inf = 999;

    painter.setPen(fg);
    painter.fillRect(position.x(), position.y(), m_cell_size.width() * atom.contents.size(), m_cell_size.height(), bg);
    painter.drawText(QRect(position.x(), position.y(), inf, inf), Qt::AlignTop, atom.contents);
}

void KakouneWidget::drawLine(QPainter& painter, RPC::Line line, QPoint position) {
    QFontMetrics font_metrics(m_font);
    for (RPC::Atom atom : line) {
        drawAtom(painter, atom, position);
        position.setX(position.x() + font_metrics.averageCharWidth() * atom.contents.size());
    }
}

void KakouneWidget::paintEvent(QPaintEvent *) {
    qDebug("Rerender kakounewidget");

    QPainter painter(this);
    painter.setFont(m_font);
    painter.fillRect(rect(), getColor(m_default_face.bg, true));

    for (int i = 0; i < m_lines.size(); ++i) {
        QPoint position(0, i * m_cell_size.height());
        drawLine(painter, m_lines[i], position);
    }
}
