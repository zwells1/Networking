#include <Zmq/PubSocket.hpp>
#include <iostream>

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void SetupPublishSocket(net::CPubSocket& PublishSocket)
{
  PublishSocket.mSignalError.connect(
    [](const std::string& ErrorMesssage)
    {
      std::cout << ErrorMessage << std::endl;

    });

  try
  {
    PublishSocket.Connect("tcp://*6999");
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

  SetupPublishSocket(PubSocket);

  while(true)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    PublishSocket.Send("Test", "Hello World");
  }

  return 0;
}
