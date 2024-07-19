#ifndef REMOTEKAKOUNESESSION_HPP
#define REMOTEKAKOUNESESSION_HPP

#include "kakounesession.hpp"
#include <QtWidgets>

class RemoteKakouneSession : public KakouneSession
{
    Q_OBJECT
  public:
    RemoteKakouneSession(QString session_id);
    ~RemoteKakouneSession();
  private:
    RemoteKakouneSession();
};

#endif
