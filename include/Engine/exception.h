#ifndef HS_ENGINE_EXCEPTION_H
# define HS_ENGINE_EXCEPTION_H

# include <exception>
# include <errno.h>
# include <string.h>

namespace hs
{
   namespace engine
   {
      /// exception code for an unknow error
      constexpr int except_unknowerror = -1;
      /// exception code for an unix error
      constexpr int except_unixerror = 0;
      /// exception code for an network invalid socket
      constexpr int except_network_invalidsocket = 1;
      /// exception code for an invalid http request
      constexpr int except_network_http_invalidrequest = 2;
      /// exception code for an unimplemented http request type
      constexpr int except_network_htpp_unimplementedrequest = 3;

      /// Base class for exceptions
      struct Exception : public std::exception
      {
         explicit Exception(const char *msg, int code = -1, int sub_code = 0) noexcept;
         virtual ~Exception() noexcept;

         /// Function to get the exception message
         virtual const char * what() const noexcept;

         /// Function to get the code of the exception
         inline int  code() noexcept {
            return (this->_code);
         }

         /// Function to get the subcode of the exception
         inline int  subcode() noexcept {
            return (this->_sub_code);
         }

      private:
         /// Code of the exception
         int         _code;

         /// Subcode of the exception
         int         _sub_code;

         /// Message of the exception
         const char  *_msg;
      };

      /// An Unknow exception
      struct UnknowException : public engine::Exception
      {
         explicit UnknowException() :
            Exception("Unknow exception", engine::except_unknowerror) {}
      };

      /// An unix exception
      struct UnixException : public engine::Exception
      {
         explicit UnixException() :
            Exception(strerror(errno), engine::except_unixerror, errno) {}
      };
   }
}

#endif
