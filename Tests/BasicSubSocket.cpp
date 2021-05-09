#include <Zmq/SubSocket.hpp>

#include <iostream>
#include <stdexcept>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void TestMessage(const std::string& MessageType, const std::string& MessageData)
{
  if (MessageType == "Test")
  {
    std::cout << "Data: " << MessageData  << std::endl;
  }
  else
  {
    std::cout << "Unknown message type: " <<
      MessageType <<
      " Message Data:  " <<
      MessageData <<
      std::endl;
  }
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SetupSubscribeSocket(CSubSocket& SubscribeSocket)
{
  SubscribeSocket.mSignalMessage.connect(
    [&](const std::string& MessageType, const std::string& MessageData)
    {
      TestMessage(MessageType, MessageData);

      std::cout << "Received Message"  << std::endl;
    });

  SubscribeSocket.mSignalError.connect(
    [&](const std::string& ErrorMessage)
    {
      std::cout << ErrorMessage << std::endl;
    });

  try
  {
    SubscribeSocket.Connect("tcp://localhost:6999");

    std::cout << "Connecting to tcp://localhost:6999" << std::endl;
  }
  catch (const zmq::error_t& Error)
  {
    std::cerr << "Error: Subscribe Socket Connect"
      << Error.what() <<
      std::endl;

    std::exit(1);
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int main()
{
  net::CSubSocket SubscribeSocket(1);

  SetupSubscribeSocket(SetupSubscribeSocket);

  while(true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(5));
  }

  return 0;
}
