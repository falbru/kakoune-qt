#ifndef KAKOUNECLI_HPP
#define KAKOUNECLI_HPP

#include <QList>
#include <QString>
#include <qdbusinterface.h>

class KakouneCli
{
  public:
    KakouneCli(const QString &service_name);

    int run(QList<QString> command);

  private:
    QDBusInterface m_dbusiface;
};

#endif
