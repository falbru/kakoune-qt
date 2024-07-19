#include "remotekakounesession.hpp"

RemoteKakouneSession::RemoteKakouneSession(QString session_id)
{
    m_session_id = session_id;
    m_process = nullptr;
}

RemoteKakouneSession::~RemoteKakouneSession()
{

}
