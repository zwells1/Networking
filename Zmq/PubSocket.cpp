#include "PubSocket.hpp"

#include <zmq.hpp>

using net::CPubSocket;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CPubSocket::CPubSocket(unsigned NumberOfI0Threads)
: CSocket(NumberOfI0Threads, ZMQ_PUB)
{
  //Prevent slow subscribers from overburdening publisher
  //TO-DO: Can make this changable in the future as well
  int MaxOutbound = 64;

  mpZmqSocket->setsockopt(ZMQ_SNDHWM, &MaxOutbound, sizeof(MaxOutbound));

  //TO-DO: Consider setting the send buffer as well. Linux is fine but Windows
  // it will be unbounded and can grow to infinity.
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CPubSocket::Connect(const std::initializer_list<const std::string>& EndPoints)
{
  try
  {
    if (maIsConnected)
    {
      mSignalError(
        "PubSocket Error: Calling bind more than once is not supported with this class");

      return;
    }

    maIsConnected = true;

    std::lock_guard<std::mutex> Lock(mMutex);

    for (const auto& EndPoint : EndPoints)
    {
      mpZmqSocket->bind(EndPoint.c_str());
    }

  }
  catch (const ::zmq::error_t&)
  {
    throw std::runtime_error("Publish Socket Connection");
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void CPubSocket::Send(
  const std::string& MessageType,
  const std::string& MessageData)
{
  try
  {
    //Prepare Message type
    ::zmq::message_t ZmqMessageType(MessageType.size());

    memcpy(ZmqMessageType.data(), MessageType.data(), MessageType.size());

    //Prepare Message Data
    ::zmq::message_t ZmqMessageData(MessageData.size());

    memcpy(ZmqMessageData.data(), MessageData.data(), MessageData.size());

    std::lock_guard<std::mutex> Lock(mMutex);

    //Send Message Type
    if (!mpZmqSocket->send(ZmqMessageType, ZMQ_SNDMORE))
    {
      mSignalError("PubSocket: Failed to send Message Type");
    }

    //Send Message Data
    if (!mpZmqSocket->send(ZmqMessageData))
    {
      mSignalError("PubSocket: Failed to send Message: " + MessageData);
    }
  }
  catch (const ::zmq::error_t& Error)
  {
    mSignalError(Error.what());
  }
}

