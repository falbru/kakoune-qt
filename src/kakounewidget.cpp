#include "kakounewidget.h"

KakouneWidget::KakouneWidget(QWidget *parent) : QWidget(parent) {
    qDebug("Constructing kakounewidget");

    m_client = new KakouneClient();
}

KakouneWidget::~KakouneWidget() {
    delete m_client;
}

void KakouneWidget::paintEvent(QPaintEvent *) {
    qDebug("Rerender kakounewidget");
}
