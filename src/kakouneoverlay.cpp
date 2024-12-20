#include "kakouneoverlay.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <qgraphicseffect.h>

KakouneOverlay::KakouneOverlay(QWidget *parent)
    : QFrame(parent), label(new QLabel(this)) {
    // Setup border and box-shadow
    auto effect = new QGraphicsDropShadowEffect;
    effect->setBlurRadius(55);
    effect->setColor(Qt::black);
    effect->setOffset(0,0);
    setGraphicsEffect(effect);

    // Setup layout and label
    auto layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("color: white; font-size: 18px;");
    label->setText("Loading...");
    layout->addWidget(label);
}

void KakouneOverlay::setMessage(const QString &message) {
    label->setText(message);
}

void KakouneOverlay::resizeEvent(QResizeEvent *event) {
    // Ensure the overlay resizes with its parent widget
    if (parentWidget()) {
        resize(parentWidget()->size());
    }
    QFrame::resizeEvent(event);
}
