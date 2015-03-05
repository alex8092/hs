#include "Network/tcpacceptor.h"
#include "Engine/exception.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

using hs::network::TcpAcceptor;
using hs::network::TcpSocket;
using hs::engine::UnixException;

TcpAcceptor::TcpAcceptor(TcpSocket &listener) :
   _listener(listener)
{
   this->_listener.setNonBlock(true);
}

TcpAcceptor::~TcpAcceptor()
{

}

bool  TcpAcceptor::hasNewSocket()
{
   struct sockaddr_in   sin;
   socklen_t            sinlen = sizeof(sin);
   int fd = ::accept(this->_listener.fd(), (struct sockaddr *)&sin, &sinlen);
   if (fd == -1 && errno != EWOULDBLOCK && errno != EAGAIN)
      throw UnixException();
   else if (fd > 0)
   {
      this->_new = new TcpSocket(fd);
      this->_new->setNonBlock(true);
      return (true);
   }
   return (false);
}
