#ifndef HS_NETWORK_TCPSOCKET_H
# define HS_NETWORK_TCPSOCKET_H

# include "Network/socket.h"
# include <string>

namespace hs
{
  namespace network
  {
    /// base class for tcp sockets
    class TcpSocket : public Socket
    {

    public:
      explicit TcpSocket() noexcept;
      explicit TcpSocket(int fd) noexcept;
      virtual ~TcpSocket() noexcept;

      /// \brief Function to connect to an host and port
      /// \param host Host to connect
      /// \param port Port on the host to connect
      /// \todo to implement
      void    connect(const std::string& host, uint16_t port);

      /// Function to listen on a specific port
      void    listen(uint16_t port, uint16_t queue_max = 10);
    };
  }
}

#endif
