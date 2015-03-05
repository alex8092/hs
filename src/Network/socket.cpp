#include "Network/socket.h"
#include "Engine/exception.h"
#include <fcntl.h>
#include <sys/ioctl.h>

using hs::network::Socket;
using hs::engine::UnixException;

Socket::Socket() noexcept :
   _fd(-1)
{

}

Socket::Socket(int fd) noexcept :
   _fd(fd)
{

}

Socket::~Socket() noexcept
{
   this->close();
}

void Socket::close () {
   if (this->_fd != -1)
   {
    ::close(this->_fd);
    this->_fd = -1;
   }
}

void  Socket::setNonBlock(bool enable)
{
   int   flags;

   if (this->_nonblock != enable)
   {
      this->_nonblock = enable;
      flags = fcntl(this->_fd, F_GETFL, 0);
      if (flags = -1)
         flags = 0;
      if (enable)
         flags |= O_NONBLOCK;
      else
         flags &= ~O_NONBLOCK;
      if (fcntl(this->_fd, F_SETFL, flags) == -1)
         throw UnixException();
   }
}

size_t               Socket::bytesAvailable() const {
   size_t   nbytes = 0;

   if (ioctl(this->_fd, FIONREAD, (char *)&nbytes) < 0)
      throw UnixException();
   return (nbytes);
}

void                 Socket::read_secure(uint32_t nread, std::stringstream& stream) const
{
   char  buffer[1024];

   while (nread > 0)
   {
      ssize_t ret = ::read(this->_fd, buffer, (nread < 1024) ? nread : 1024);
      if (ret == -1)
         throw UnixException();
      else if (ret > 0) {
         stream.write(buffer, ret);
         nread -= ret;
      }
   }
}

std::stringstream&   Socket::read(uint16_t size, std::stringstream& stream)
{
   char  buffer[size + 1];


}
