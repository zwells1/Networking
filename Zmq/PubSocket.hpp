#pragma once

#include "Socket.hpp"

namespace net
{
  class CPubSocket : public net::CSocket
  {
    public:

      CPubSocket(unsigned NumberOfI0Threads);

      virtual void Connect(
        const std::initializer_list<const std::string>& EndPoints) override;

      void Send(
        const std::string& MessageType,
        const std::string& MessageData);
  };
}

