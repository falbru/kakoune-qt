#ifndef KAKOUNECONTENT_HPP
#define KAKOUNECONTENT_HPP

#include "drawoptions.hpp"
#include "rpc/line.hpp"
#include <QtWidgets>

class KakouneContent : public QWidget {
  Q_OBJECT

  public:
    KakouneContent(QList<RPC::Line> lines, RPC::Face default_face, DrawOptions *draw_options, QWidget *parent = nullptr);
    ~KakouneContent();

    QList<RPC::Line> getLines();
    void setLines(QList<RPC::Line> lines);

    RPC::Face getDefaultFace();
    void setDefaultFace(RPC::Face face);

  protected:
    void paintEvent(QPaintEvent *ev) override;

  private:
    QList<RPC::Line> m_lines;
    RPC::Face m_default_face;
    DrawOptions* m_draw_options;
};

#endif
