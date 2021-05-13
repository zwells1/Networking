#include <Zmq/PubSocket.hpp>
#include <zmq.hpp>
#include <iostream>
#include <thread>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SetupPublishSocket(net::CPubSocket& PublishSocket)
{
  PublishSocket.mSignalError.connect(
    [](const std::string& ErrorMessage)
    {
      std::cout << ErrorMessage << std::endl;

    });

  try
  {
    PublishSocket.Connect({"tcp://*:7999"});

    std::cout << "Pub: tcp://*:7999" << std::endl;
  }
  catch (const zmq::error_t& Error)
  {
    std::cerr << "Error: Publish Socket Connect"
      << Error.what() <<
      std::endl;

    std::exit(1);
  }
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
int main()
{
  net::CPubSocket PublishSocket(1);

  SetupPublishSocket(PublishSocket);

  std::this_thread::sleep_for(std::chrono::seconds(1));

  while(true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    PublishSocket.Send("Test", "Hello World");
  }

  return 0;
}
