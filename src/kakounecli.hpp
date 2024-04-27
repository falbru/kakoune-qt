#ifndef KAKOUNECLI_HPP
#define KAKOUNECLI_HPP

#include <QList>
#include <QLocalSocket>
#include <QString>

class KakouneCli
{
  public:
    KakouneCli();
    ~KakouneCli();

    int run(QStringList command);

  private:
    QLocalSocket *m_socket;
};

#endif
