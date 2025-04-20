#ifndef KAKOUNELISTCONTENT_HPP
#define KAKOUNELISTCONTENT_HPP

#include "kakounecontent.hpp"
#include "rpc/face.hpp"
#include <QtWidgets>

class KakouneListContent : public KakouneContent
{
    Q_OBJECT

  public:
    KakouneListContent(DrawOptions *draw_options, RPC::Face default_face, QWidget *parent = nullptr);
    ~KakouneListContent();

    void setContent(QList<RPC::Line> content);

    void setSelectedItem(int selected_item);
    int getSelectedItem() const;

    void setSelectedFace(RPC::Face selected_face);
    const RPC::Face &getSelectedFace() const;

  protected:
    void paintEvent(QPaintEvent *ev) override;

  private:
    int m_selected_item;
    RPC::Face m_selected_face;
};

#endif
