#pragma once

#include <thread>

namespace net
{
  class CSubSocket : public CSocket
  {
    public:

      CSubSocket(unsigned NumberOfI0Threads);

      ~CSubSocket();

      virtual void Connect(
        const std::initializer_list<const std::string>& EndPoints) override;

      //TO-DO: add this in
//      void SetReceiveTimeout(unsigned TimeoutMs);

      //TO-DO: add this in
      //void SetReceiverBufferSize(unsigned BufferSize);

      //TO-DO: add this in
      //void SetReceiveQueueSize(unsigned MaxNumberOfMessages);

      void SubscribeToType(const std::string& MessageType);

      void SubscribeToEverything();

      void ClearSubscriptions();

      boost::signals::signal<
        void(const std::string& MessageType, const std::string& MessageData)> mSignalMessage;

    private:

      void Receive(std::string& MessageType, std::string& MessageData);

      std::atomic<bool> maIsRunning;

      std::unique_ptr<std::thread> mpThread;
  };
}

