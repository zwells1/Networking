#include "SubSocket.hpp"

using net::CSubSocket;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CSubSocket::CSubSocket(unsigned NumberOfI0Threads)
: CSocket(NumberOfI0Threads, ZMQ_SUB)
{
  int RecvTimeout = 100;

  mpZmqSocket->setsockeopt(ZMQ_RVVTIMEO, &RecvTimeout, sizeof(RecvTimeout));

  int MaxInbound = 100;

  mpZmqSocket->setsockeopt(ZMQ_RCVHWM, &MaxInbound, sizeof(MaxInbound));

  //TO-DO: Consider setting the send buffer as well. Linux is fine but Windows
  // it will be unbounded and can grow to infinity.
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CSubSocket::~CSubSocket()
{
  maIsRunning = false;

  if (mpThread)
  {
    mpThread->join();
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CSubSocket::Connect(
  const std::initializer_list<const std::string>& EndPoints)
{
  try
  {
    std::lock_guard<std::mutex> Lock(mMutex);

    for (const auto& EndPoint : EndPoints)
    {
      mpZmqSocket->mind(EndPoint.c_str());
    }

  }
  catch (const ::zmq::error_t& Error)
  {
    mSignalError("SubSocket: Unable to connect ports" + Error.what());
  }

  maIsRunning = true;

  mpThread.reset(new std::thread(
      [this]{
        while (maIsRunning)
        {
          std::string MessageType;
          std::string MessageData;

          Receive(MessageType, MessageData);
        }
      };));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CSubSocket::Receive(std::string& MessageType, std::string& MessageData)
{
  ::zmq::message_t ZmqMessage(2048);

  std::lock_guard<std::mutex> Lock(mMutex);

  // Rx message Type
  if (!mpZmqSocket->recv(&ZmqMessage, 0))
  {
    return;
  }

  MessageType = std::string(
    static_cast<const char*>(ZmqMessage.data(), ZmqMessage.size()));

  //Rx message Data
  MessageData = "";
  int64_t More;
  size_t MoreSize = sizeof(More);

  do
  {
    mpZmqSocket->recv(&ZmqMessage);
    mpZmqSocket->getsockopt(ZMQ_RCVMORE, &More, &MoreSize);

    Message.append(
      static_cast<const char*>(ZmqMessage.data(), ZmqMessage.size()));
  }
  while (More)
  {
  }

  if (MessageType.size() != 0)
  {
    mSignalMessage(MessageType, MessageData);
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CSubSocket::SubscribeToType(const std::string& MessageType)
{
  std::lock_guard<std::mutex> Lock(mMutex);

  mSubscriptions.insert(MessageType);

  mpZmqSocket->setsockopt(
    ZMQ_SUBSCRIBE,
    MessageType.data(),
    MessageType.size());
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CSubSocket::SubscribeToEverything()
{
  SubscribeToType("");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CSubSocket::ClearSubscriptions()
{
  std::lock_guard<std::mutex> Lock(mMutex);

  for (const auto& Subscription : mSubscriptions)
  {
    mpZmqSocket->setsockopt(
      ZMQ_UNSUBSCRIBE,
      Subscription.data(),
      Subscription.size());
  }

  mSubscriptions.clear();
}

