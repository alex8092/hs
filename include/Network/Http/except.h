#ifndef HS_NETWORK_HTTP_EXCEPT_H
# define HS_NETWORK_HTTP_EXCEPT_H

# include "Engine/exception.h"

namespace hs
{
   namespace network
   {
      namespace http
      {
         struct InvalidRequestException : public engine::Exception {
            explicit InvalidRequestException() :
               Exception("Invalid http request", engine::except_network_http_invalidrequest) {}
         };
         
         struct UnimplementedRequestException : public engine::Exception {
            explicit UnimplementedRequestException() :
               Exception("Unimplemented http request type", engine::except_network_htpp_unimplementedrequest) {}
         };
      }
   }
}

#endif
