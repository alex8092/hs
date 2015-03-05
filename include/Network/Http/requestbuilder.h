#ifndef HS_NETWORK_HTTP_REQUESTBUILDER_H
# define HS_NETWORK_HTTP_REQUESTBUILDER_H

# include "Network/tcpsocket.h"
# include "Network/Http/request.h"
# include <sstream>
# include <list>

namespace hs
{
   namespace network
   {
      namespace http
      {
         class RequestBuilder
         {
         private:
            std::list<std::pair<char*,size_t>>        _options;
            std::string             _request_line;
            TcpSocket               *_sock = nullptr;
            Request                 *_req = nullptr;
            std::stringstream       _raw_request;
            bool                    _is_request_line = true;
            bool                    _has_datas = false;
            size_t                  _datas_length = 0;
            bool                    _is_header = true;
            bool                    _end = false;
            int                     _indicator = 0;
            std::streampos          _last_pos = 0;
            std::streampos          _begin_data = 0;

            void                    _parse_uri(char *buffer, size_t len);
            void                    _parse_request_line(char *buffer, size_t len);

         public:
            explicit RequestBuilder(TcpSocket& sock);
            virtual  ~RequestBuilder();

            void           continueBuild();
            inline bool    isEnd() const {
               return (this->_end);
            }
         };
      }
   }
}

#endif
