#ifndef HS_NETWORK_TCPACCEPTOR_H
# define HS_NETWORK_TCPACCEPTOR_H

# include "Network/tcpsocket.h"

namespace hs
{
   namespace network
   {
      class TcpAcceptor
      {
      private:
         TcpSocket   &_listener;
         TcpSocket   *_new = nullptr;

      public:
         explicit TcpAcceptor(TcpSocket& listener);
         virtual  ~TcpAcceptor();

         bool               hasNewSocket();
         inline TcpSocket*  newSocket() const {
            return (this->_new);
         }
      };
   }
}

#endif
