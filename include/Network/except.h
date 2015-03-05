#ifndef HS_NETWORK_EXCEPT_H
# define HS_NETWORK_EXCEPT_H

# include "Engine/exception.h"

namespace hs
{
   namespace network
   {
      /// Structure to define an invalid socket exception
      struct InvalidSocketException : public engine::Exception
      {
         explicit InvalidSocketException() :
            Exception("Invalid socket", engine::except_network_invalidsocket) {}
      };
   }
}

#endif
