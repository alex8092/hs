#ifndef HS_NETWORK_SOCKET_H
# define HS_NETWORK_SOCKET_H

# include <unistd.h>
# include <sstream>

namespace hs
{
  namespace network
  {
    /// Base class for sockets
    class Socket
    {
    protected:
      /// File descriptor (socket)
      int   _fd;
      /// Boolean to indicate non blocking state
      bool  _nonblock = false;

    public:
      explicit  Socket() noexcept;
      explicit  Socket(int fd) noexcept;
      virtual   ~Socket() noexcept;

      /// Function to close the socket
      void close ();

      /// Function to know if the socket is valid
      inline bool isValid() const {
        return (this->_fd != -1);
      }

      /// Function to get the socket descriptor
      inline int  fd() const {
        return (this->_fd);
      }

      void        setNonBlock(bool enable);

      void        read_secure(uint32_t nread, std::stringstream& stream) const;

      /// Function to read an buffer from the socket
      virtual std::stringstream& read(uint16_t size, std::stringstream& stream);

      size_t   bytesAvailable() const;
    };
  }
}

#endif
