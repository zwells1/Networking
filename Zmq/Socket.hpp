#pragma once

#include <boost/signals2/signal.hpp>
#include <atomic>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <set>

namespace zmq
{
  class context_t;
  class socket_t;
}

namespace net
{
  class CSocket
  {
    public:

      virtual ~CSocket();

      void SetLinger(unsigned LingerMs);

      virtual void Connect(
        const std::initializer_list<const std::string>& EndPoints) = 0;

      boost::signals2::signal<void(const std::string& Error)> mSignalError;

    protected:

      CSocket(unsigned NumberOfI0Threads, int SocketType);

      std::unique_ptr<::zmq::context_t> mpContext;

      std::unique_ptr<::zmq::socket_t> mpZmqSocket;

      std::atomic<bool> maIsConnected;

      //Subscriber parameters
      //TO-DO: might want to consider just a set not a multiset
      std::multiset<std:string> mSubscriptions;

      std::mutex mMutex;
  };
}

