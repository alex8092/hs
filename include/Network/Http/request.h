#ifndef HS_NETWORK_HTTP_REQUEST_H
# define HS_NETWORK_HTTP_REQUEST_H

# include "Network/tcpsocket.h"

namespace hs
{
   namespace network
   {
      namespace http
      {
         enum class RequestType {
            GET,
            POST,
            HEAD
         };

         class Request
         {
         private:
            TcpSocket   *_sock;
            int         _major;
            int         _minor;
            RequestType _type;

         public:
            explicit       Request(TcpSocket& sock, int major = 0, int minor = 9);
            virtual        ~Request();

            inline void    setType(RequestType type) {
               this->_type = type;
            }
         };
      }
   }
}

#endif
