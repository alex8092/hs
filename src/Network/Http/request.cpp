#include "Network/Http/request.h"

using hs::network::http::Request;
using hs::network::TcpSocket;

Request::Request(TcpSocket& sock, int major, int minor) :
   _sock(&sock),
   _major(major),
   _minor(minor)
{

}

Request::~Request()
{

}
