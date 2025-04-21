#ifndef KAKOUNECONTENT_HPP
#define KAKOUNECONTENT_HPP

#include "abstractkakounecontent.hpp"
#include "drawoptions.hpp"
#include "rpc/line.hpp"
#include <QtWidgets>

class KakouneContent : public AbstractKakouneContent
{
    Q_OBJECT

  public:
    KakouneContent(DrawOptions *draw_options, RPC::Face default_face, QWidget *parent = nullptr);
    ~KakouneContent();

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;

    void setContent(QList<RPC::Line> content) override;
    const QList<RPC::Line> &getContent() const override;

    void setDefaultFace(RPC::Face default_face) override;
    const RPC::Face &getDefaultFace() const override;

  protected:
    void paintEvent(QPaintEvent *ev) override;

  private:
    QList<RPC::Line> m_content;
    DrawOptions *m_draw_options;
    RPC::Face m_default_face;
};

#endif
