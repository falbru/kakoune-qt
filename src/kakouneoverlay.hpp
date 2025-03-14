#ifndef KAKOUNEOVERLAY_HPP
#define KAKOUNEOVERLAY_HPP

#include "rpc/face.hpp"
#include <QtWidgets>

class KakouneOverlay : public QFrame
{
    Q_OBJECT
  public:
    KakouneOverlay(QWidget *parent = nullptr);
    ~KakouneOverlay();

    void updateOverlay();
    void setFace(RPC::Face face);

  private:
    void moveToFitParent();
    void resizeToFitParent();
};

#endif
