#ifndef KAKOUNEWINDOW_HPP
#define KAKOUNEWINDOW_HPP

#include "kakouneclient.hpp"
#include <QtWidgets>

class KakouneWidget : public QWidget
{
    Q_OBJECT
  public:
    KakouneWidget(const QString& session_id, QWidget *parent = nullptr);
    ~KakouneWidget();

  protected:
    void paintEvent(QPaintEvent *ev) override;
    void keyPressEvent(QKeyEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void resizeEvent(QResizeEvent *ev) override;
  private slots:
    void refresh();
  signals:
    void finished();

  private:
    void setFont(const QString &font_name, int font_size);

    KakouneClient *m_client;

    QFont m_font;
    QSize m_cell_size;
};

#endif
