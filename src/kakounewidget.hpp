#ifndef KAKOUNEWINDOW_HPP
#define KAKOUNEWINDOW_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include "kakouneinfobox.hpp"
#include "kakounemenu.hpp"
#include <QtWidgets>

class TabIgnoreFilter: public QObject
{
  Q_OBJECT
  protected:
    bool eventFilter(QObject *object, QEvent *event);
};

class KakouneWidget : public QWidget
{
    Q_OBJECT
  public:
    KakouneWidget(const QString &session_id, DrawOptions *draw_options, QWidget *parent = nullptr);
    ~KakouneWidget();

    void pressKey(int key_code, Qt::KeyboardModifiers modifiers);

    KakouneClient *getClient();

  protected:
    void paintEvent(QPaintEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void resizeEvent(QResizeEvent *ev) override;
  private slots:
    void clientRefreshed();
  signals:
    void finished();
    void refresh();

  private:
    KakouneClient *m_client;

    KakouneMenu *m_menu;
    KakouneInfoBox *m_info_box;

    DrawOptions *m_draw_options;
};

#endif
