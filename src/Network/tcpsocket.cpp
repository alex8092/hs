/// \file

#include "Network/tcpsocket.h"
#include "Network/except.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>

using hs::network::Socket;
using hs::network::TcpSocket;
using hs::engine::UnixException;
using hs::network::InvalidSocketException;

TcpSocket::TcpSocket() noexcept :
   Socket(::socket(AF_INET, SOCK_STREAM, 0))
{

}

TcpSocket::TcpSocket(int fd) noexcept :
   Socket(fd)
{

}

TcpSocket::~TcpSocket() noexcept
{

}

void  TcpSocket::connect(const std::string& host, uint16_t port)
{
   struct hostent       *hp;
   struct sockaddr_in   sin;

   if (!Socket::isValid())
      throw InvalidSocketException();
   if ((hp = ::gethostbyname(host.c_str())) == nullptr)
      throw UnixException();
   ::bzero(&sin, sizeof(sin));
   sin.sin_family = AF_INET;
   ::bcopy(hp->h_addr, &sin.sin_addr.s_addr, hp->h_length);
   sin.sin_port = htons(port);
   if (::connect(Socket::_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
      throw UnixException();
}

void  TcpSocket::listen(uint16_t port, uint16_t max_queue)
{
   int optval = 1;
   struct sockaddr_in sin;

   if (!Socket::isValid())
    throw InvalidSocketException();
   if (::setsockopt(Socket::_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(optval)) < 0)
    throw UnixException();
   ::bzero(&sin, sizeof(sin));
   sin.sin_port = ::htons(port);
   sin.sin_family = AF_INET;
   sin.sin_addr.s_addr = ::htonl(INADDR_ANY);
   if (::bind(Socket::_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    throw UnixException();
   if (::listen(Socket::_fd, max_queue) < 0)
    throw UnixException();
}
