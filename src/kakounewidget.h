#ifndef KAKOUNEWINDOW_H
#define KAKOUNEWINDOW_H

#include "kakouneclient.h"
#include <QtWidgets>

class KakouneWidget : public QWidget {
    Q_OBJECT
public:
    KakouneWidget(QWidget* parent=nullptr);
    ~KakouneWidget();
protected:
	void paintEvent(QPaintEvent* ev) override;
private:
    KakouneClient* m_client;
};

#endif
