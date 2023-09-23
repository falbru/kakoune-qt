#include "kakounewidget.h"
#include "rpc.h"

void KakouneWidget::handleRequest(QJsonObject request) {
    const QString method = request["method"].toString();
    QJsonArray request_params = request["params"].toArray();

    if (method == "draw") {
        RPC::DrawRequest request = RPC::deserializeDrawRequest(request_params);
        for (auto line : request.lines) {
            qDebug("NEW LINE");
            for (auto atom : line) {
                qDebug() << atom.contents << atom.face.fg << atom.face.bg;
            }
        }
        // TODO
    }else {
        qDebug() << "Unkown method: " << method;
    }
}

KakouneWidget::KakouneWidget(QWidget *parent) : QWidget(parent) {
    qDebug("Constructing kakounewidget");

    m_client = new KakouneClient();
    connect(m_client, &KakouneClient::handleRequest, this, &KakouneWidget::handleRequest);
}

KakouneWidget::~KakouneWidget() {
    delete m_client;
}

void KakouneWidget::paintEvent(QPaintEvent *) {
    qDebug("Rerender kakounewidget");
}
