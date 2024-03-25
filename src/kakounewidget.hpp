#ifndef KAKOUNEWINDOW_HPP
#define KAKOUNEWINDOW_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include "kakouneinfobox.hpp"
#include "kakounemenu.hpp"
#include "kakounetextedit.hpp"
#include <QtWidgets>

class KakouneWidget : public QWidget
{
    Q_OBJECT
  public:
    KakouneWidget(const QString &session_id, DrawOptions *draw_options, QWidget *parent = nullptr);
    KakouneWidget(const QString &session_id, DrawOptions *draw_options, QString client_arguments,
                  QWidget *parent = nullptr);
    ~KakouneWidget();

    KakouneClient *getClient();

    QSize sizeHint() const override;

  private slots:
    void clientRefreshed();
  signals:
    void finished();
    void refresh();

  private:
    KakouneClient *m_client;

    KakouneTextEdit *m_textedit;
    KakouneMenu *m_menu;
    KakouneInfoBox *m_info_box;

    DrawOptions *m_draw_options;
};

#endif
