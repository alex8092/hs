#include <iostream>
#include "Engine/exception.h"
#include "Network/tcpsocket.h"
#include "Network/tcpacceptor.h"
#include "Network/Http/requestbuilder.h"
#include "IO/waiter.h"
#include <unistd.h>

hs::io::Waiter  w;

bool  on_read(int fd, void *data)
{
   hs::network::http::RequestBuilder   *builder = (hs::network::http::RequestBuilder *)data;

   try
   {
      builder->continueBuild();
      if (builder->isEnd())
      {
         w.add(false, fd, data);
         return (true);
      }
      return (false);
   }
   catch (hs::engine::Exception& e)
   {
      std::cerr << e.what() << std::endl;
      return (true);
   }

}

bool  on_write(int fd, void *data)
{
   static std::string         msg;
   static bool                init = false;

   if (!init)
   {
      std::stringstream ss;
      const char  *content = "<!doctype><html><head><title>bonjour</title></head><body><h1>Bonjour</h1></body></html>";
      ss << "HTTP/1.1 OK\r\nContent-Type: text/html\r\nContent-Length: " << ::strlen(content) << "\r\n\r\n";
      ss.write(content, strlen(content));
      msg = ss.str();
   }
   ::write(fd, msg.c_str(), msg.length());
   delete (hs::network::http::RequestBuilder *)data;
   return (true);
}

int main(void)
{
   hs::network::TcpSocket     s;
   hs::network::TcpSocket     *cur;
   hs::network::http::RequestBuilder   *builder;
   hs::network::TcpAcceptor   accept(s);

   w.setOnRead(on_read);
   w.setOnWrite(on_write);
   try
   {
      s.listen(4545);
      while (1)
      {
         while (accept.hasNewSocket())
         {
            cur = accept.newSocket();
            builder = new hs::network::http::RequestBuilder(*cur);
            w.add(true, cur->fd(), (void *)builder);
         }
         if (w.processEvents())
         {

         }
      }
   }
   catch (hs::engine::Exception& e)
   {
      std::cerr << "except: " << e.what() << std::endl;
      return (1);
   }
   return (0);
}
