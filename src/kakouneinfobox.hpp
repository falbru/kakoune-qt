#ifndef KAKOUNEINFOBOX_HPP
#define KAKOUNEINFOBOX_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include "kakounecontent.hpp"
#include "kakounemenu.hpp"
#include "kakouneoverlay.hpp"
#include <QtWidgets>

class KakouneInfoBox : public KakouneOverlay
{
    Q_OBJECT
  public:
    KakouneInfoBox(KakouneClient *client, KakouneMenu *menu, DrawOptions *draw_options, QWidget *parent = nullptr);
    ~KakouneInfoBox();

  private slots:
    void showInfoBox();

  private:
    KakouneClient *m_client;
    KakouneMenu *m_menu;
    DrawOptions *m_draw_options;
    KakouneContent *m_title;
    QFrame *m_title_separator;
    KakouneContent *m_content;

    void applyPromptStyle();
    void applyInlineStyle(int type);
    void applyMenuDocStyle();
    void applyModalStyle();
};

#endif
