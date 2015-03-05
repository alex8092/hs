#include "Network/Http/requestbuilder.h"
#include "Network/except.h"
#include "Network/Http/except.h"
#include "Engine/exception.h"
#include <iostream>

using hs::network::http::RequestBuilder;
using hs::network::TcpSocket;
using hs::network::InvalidSocketException;
using hs::network::http::InvalidRequestException;
using hs::network::http::UnimplementedRequestException;
using hs::network::http::RequestType;
using hs::engine::UnknowException;

RequestBuilder::RequestBuilder(TcpSocket& sock) :
   _sock(&sock)
{

}

RequestBuilder::~RequestBuilder()
{
   for (auto& it : this->_options) {
      delete [] it.first;
   }
   delete this->_sock;
}

void RequestBuilder::_parse_uri (char *buffer, size_t len) {
   std::cout << "uri: " << buffer << std::endl;
}

void RequestBuilder::_parse_request_line (char *buffer, size_t len) {
   RequestType type;
   size_t      pos = 0;

   if (!strncasecmp(buffer, "GET ", 4)) {
      type = RequestType::GET;
      pos += 4;
   }
   else if (!strncasecmp(buffer, "POST ", 5)) {
      type = RequestType::POST;
      pos += 5;
   }
   else if (!strncasecmp(buffer, "HEAD ", 5)) {
      type = RequestType::HEAD;
      pos += 5;
   }
   else
      throw UnimplementedRequestException();
   this->_parse_uri(buffer + pos, len - pos);
   this->_is_request_line = false;
}

void RequestBuilder::continueBuild () {
   size_t   bytes_to_read;

   if (!this->_sock)
      throw InvalidSocketException();
   bytes_to_read = this->_sock->bytesAvailable();
   if (bytes_to_read == 0)
      throw InvalidRequestException();
   this->_sock->read_secure(bytes_to_read, this->_raw_request);
   while (this->_is_header && bytes_to_read)
   {
      int c = this->_raw_request.get();
      if (c == -1)
         throw UnknowException();
      if (c == '\r' || c == '\n')
         ++this->_indicator;
      else
      {
         if (this->_indicator == 2)
         {
            std::streampos current_pos = this->_raw_request.tellg();
            ssize_t        len = current_pos - this->_last_pos;
            char           *buffer;

            if (len < 2)
               throw UnknowException();
            buffer = new char[len + 1];
            this->_raw_request.seekg(-len, std::ios_base::cur);
            this->_raw_request.read(buffer, len);
            len -= 2;
            buffer[len] = 0;
            if (this->_is_request_line)
               this->_parse_request_line(buffer, len);
            else
               break ;
            this->_last_pos = (size_t)current_pos - 1;
         }
         this->_indicator = 0;
      }
      --bytes_to_read;
   }
   // if (this->_is_options)
   //    this->_raw_request.seekg(-bytes_to_read, std::ios_base::end);
   // while (this->_is_options && bytes_to_read)
   // {
   //    c = this->_raw_request.get();
   //    if (c == '\r' || c == '\n')
   //       ++this->_final;
   //    else {
   //       if (this->_final == 2) {
   //          std::streampos cur = this->_raw_request.tellg();
   //          ssize_t         len = cur - this->_last_pos;
   //          if (len > 2) {
   //             char  *buffer = new char[len + 1];
   //             this->_raw_request.seekg(this->_last_pos);
   //             this->_raw_request.read(buffer, len);
   //             len -= 2;
   //             buffer[len] = 0;
   //             if (!this->_is_request_line)
   //             {
   //                char  *index = ::strchr(buffer, ':');
   //                std::cout << "index: " << index << std::endl;
   //                if (index == nullptr)
   //                   throw InvalidRequestException();
   //                if (!::strncmp(buffer, "Content-Length", index - buffer))
   //                {
   //                   this->_has_datas = true;
   //                   this->_datas_length = ::atoi(index + 1);
   //                }
   //                else
   //                {
   //                   this->_options.push_back(std::make_pair(buffer, len));
   //                }
   //             }
   //             else
   //             {
   //                this->_request_line = std::string(buffer, len);
   //                this->_is_request_line = false;
   //             }
   //          }
   //          this->_last_pos = (size_t)cur - 1;
   //       }
   //       this->_final = 0;
   //    }
   //    if (this->_final == 4)
   //    {
   //       this->_begin_data = this->_raw_request.tellg();
   //       this->_is_options = false;
   //    }
   //    --bytes_to_read;
   // }
   // if (!this->_is_options && !this->_has_datas)
   //    this->_end = true;
   // else if (!this->_is_options && this->_has_datas)
   // {
   //    std::cout << "waiting: " << this->_raw_request.tellp() - this->_begin_data << std::endl;
   //    if (this->_raw_request.tellg() - this->_begin_data >= this->_datas_length)
   //    {
   //       std::cout << "datas: " << this->_raw_request.str().substr(this->_begin_data) << std::endl;
   //       this->_end = true;
   //    }
   // }
   throw InvalidRequestException();
}
