#ifndef KAKOUNEOVERLAY_HPP
#define KAKOUNEOVERLAY_HPP

#include "rpc/face.hpp"
#include <QtWidgets>

enum Alignment
{
    ABOVE,
    BELOW,
    LEFT,
    RIGHT,
};

class KakouneOverlay : public QFrame
{
    Q_OBJECT
  public:
    KakouneOverlay(QWidget *parent = nullptr);
    ~KakouneOverlay();

    void updateOverlay();
    void setFace(RPC::Face face);

  protected:
    QRect anchor;
    int position_from_anchor;
    int alignment_from_anchor;

  private:

    void moveToFitParent();
    void resizeToFitParent();
};

#endif
