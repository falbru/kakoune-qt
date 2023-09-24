#ifndef KAKOUNEWINDOW_H
#define KAKOUNEWINDOW_H

#include "kakouneclient.h"
#include "rpc.h"
#include <QtWidgets>

class KakouneWidget : public QWidget {
    Q_OBJECT
public:
    KakouneWidget(QWidget* parent=nullptr);
    ~KakouneWidget();
protected:
	void paintEvent(QPaintEvent* ev) override;
	void keyPressEvent(QKeyEvent* ev) override;
	void resizeEvent(QResizeEvent* ev) override;
private slots:
    void handleRequest(QJsonObject request);
private:
    void setFont(const QString& font_name, int font_size);

    QColor getColor(RPC::Color color, bool bg);
    void drawAtom(QPainter& painter, RPC::Atom atom, QPoint position);
    void drawLine(QPainter& painter, RPC::Line line, QPoint position);

    KakouneClient* m_client;

    QFont m_font;
    QSize m_cell_size;

    QList<RPC::Line> m_lines;
    RPC::Face m_default_face;
    RPC::Face m_padding_face;
};

#endif
