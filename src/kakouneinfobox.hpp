#ifndef KAKOUNEINFOBOX_HPP
#define KAKOUNEINFOBOX_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include "kakounemenu.hpp"
#include <QtWidgets>

enum class InlineType
{
    ANY,
    ABOVE,
    BELOW,
};

class KakouneInfoBox : public QWidget
{
    Q_OBJECT
  public:
    KakouneInfoBox(KakouneClient *client, KakouneMenu *menu, DrawOptions *draw_options, QWidget *parent = nullptr);
    ~KakouneInfoBox();

  protected:
    void paintEvent(QPaintEvent *ev) override;

  private slots:
    void showInfoBox();

  private:
    KakouneClient *m_client;
    KakouneMenu *m_menu;
    DrawOptions *m_draw_options;

    void resizeToFitContent();

    void applyPromptStyle();
    void applyInlineStyle(InlineType type);
    void applyMenuDocStyle();
    void applyModalStyle();
};

#endif
