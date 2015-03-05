#include "Engine/exception.h"

using hs::engine::Exception;

Exception::Exception(const char *msg, int code, int sub_code) noexcept :
  std::exception(),
  _code(code),
  _sub_code(sub_code),
  _msg(msg)
{

}

Exception::~Exception() noexcept
{

}

const char  *Exception::what() const noexcept
{
  return (this->_msg);
}
