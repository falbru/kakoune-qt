#include "kakouneoverlay.hpp"
#include "rpc/face.hpp"

KakouneOverlay::KakouneOverlay(QWidget *parent) : QFrame(parent)
{
    this->setAutoFillBackground(true);

    this->setFrameStyle(QFrame::Panel | QFrame::Plain);
    this->setFrameShadow(QFrame::Plain);
    this->setLineWidth(1);
}

KakouneOverlay::~KakouneOverlay()
{
}

void KakouneOverlay::setFace(RPC::Face face)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, face.getFgAsQColor(ColorPalette()));
    pal.setColor(QPalette::Window, face.getBgAsQColor(ColorPalette()));
    setPalette(pal);
}

void KakouneOverlay::updateOverlay()
{
    resizeToFitParent();
    moveToFitParent();
}

void KakouneOverlay::resizeToFitParent()
{
    int width = this->width();
    int height = this->height();

    if (width > parentWidget()->width())
    {
        width = parentWidget()->width();
        height = heightForWidth(width);
    }

    resize(width, height);
}

void KakouneOverlay::moveToFitParent()
{
}
