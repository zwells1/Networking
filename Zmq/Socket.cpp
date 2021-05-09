#include "Socket.hpp"

using net::CSocket;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CSocket::CSocket(unsigned NumberOfI0Threads, int SocketType)
: mpContext(new ::zmq::context_t (NumberOfI0Threads)),
  mpZmqSocket(new ::zmq::socket_t (*mpContext, SocketType))
{
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CSocket::~CSocket()
{
  if (mpZmqSocket->getsockopt<int>(ZMQ_LINGER) != int(1000))
  {
    SetLinger(1000);
  }

  mpZmqSocket->close();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CSocket::SetLinger(unsigned LingerMs)
{
  std::lock_guard<std::mutex> Lock(mMutex);

  mpZmqSocket->setsockopt(ZMQ_LINGER, &LingerMs, sizeof(LingerMs));
}

