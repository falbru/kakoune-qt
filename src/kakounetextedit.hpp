#ifndef KAKOUNETEXTEDIT_HPP
#define KAKOUNETEXTEDIT_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include <QtWidgets>

class TabIgnoreFilter : public QObject
{
    Q_OBJECT
  protected:
    bool eventFilter(QObject *object, QEvent *event);
};

class KakouneTextEdit : public QWidget
{
    Q_OBJECT

  public:
    KakouneTextEdit(KakouneClient *client, DrawOptions *draw_options, QWidget *parent = nullptr);
    ~KakouneTextEdit();

    void pressKey(int key_code, Qt::KeyboardModifiers modifiers);

    void setActiveClient(KakouneClient *client);

  protected:
    void paintEvent(QPaintEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void resizeEvent(QResizeEvent *ev) override;

  private:
    KakouneClient *m_client;
    DrawOptions *m_draw_options;
};

#endif
