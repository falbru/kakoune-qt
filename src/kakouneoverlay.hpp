#ifndef KAKOUNEOVERLAY_HPP
#define KAKOUNEOVERLAY_HPP

#include <QFrame>

class QLabel;

class KakouneOverlay : public QFrame {
    Q_OBJECT

public:
    explicit KakouneOverlay(QWidget *parent = nullptr);
    void setMessage(const QString &message);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *label;
};

#endif // KAKOUNEOVERLAY_H
