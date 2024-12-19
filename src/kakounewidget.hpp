#ifndef KAKOUNEWINDOW_HPP
#define KAKOUNEWINDOW_HPP

#include "drawoptions.hpp"
#include "kakouneclient.hpp"
#include "kakouneinfobox.hpp"
#include "kakounemenu.hpp"
#include "kakounetextedit.hpp"
#include <QtWidgets>
#include <qevent.h>

class KakouneWidget : public QWidget
{
    Q_OBJECT
  public:
    KakouneWidget(const QString &session_id, const QUuid &window_id, DrawOptions *draw_options,
                  QWidget *parent = nullptr);
    KakouneWidget(const QString &session_id, const QUuid &window_id, DrawOptions *draw_options,
                  QString client_arguments, QWidget *parent = nullptr);
    ~KakouneWidget();

    void installEventFilter(QObject *filter);

    QUuid getID();
    KakouneClient *getClient();

    QSize sizeHint() const override;

    static KakouneWidget *findParentKakouneWidget(QWidget *widget);

    void setVisible(bool visible) override;

  private slots:
    void clientRefreshed();
    void setUIOptions(QMap<QString, QString> options);
  signals:
    void finished();
    void refresh();
    void changedVisibility(bool visible);

  private:
    QUuid m_id;
    KakouneClient *m_client;

    KakouneTextEdit *m_textedit;
    KakouneMenu *m_menu;
    KakouneInfoBox *m_info_box;

    DrawOptions *m_draw_options;
};

#endif
