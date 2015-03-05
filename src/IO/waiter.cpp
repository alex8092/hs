#include "IO/waiter.h"
#include "Engine/exception.h"

using hs::io::Waiter;
using hs::engine::UnixException;

Waiter::Waiter() noexcept :
   _max(-1)
{
   FD_ZERO(&this->_master_read);
   FD_ZERO(&this->_master_write);
   FD_ZERO(&this->_master_except);
   FD_ZERO(&this->_currently_reads);
   FD_ZERO(&this->_currently_writes);
   FD_ZERO(&this->_currently_excepts);
}

Waiter::~Waiter() noexcept
{

}

void  Waiter::add(bool toread, int fd, void *data)
{
   if (toread)
   {
      FD_SET(fd, &this->_master_read);
      this->_to_read.push_back(std::make_pair(fd, data));
   }
   else
   {
      FD_SET(fd, &this->_master_write);
      this->_to_write.push_back(std::make_pair(fd, data));
   }
   FD_SET(fd, &this->_master_except);
   if (fd > this->_max)
      this->_max = fd;
}

bool  Waiter::processEvents()
{
   static struct timeval   timeout;
   static bool             init = false;
   int                     res;

   if (!this->_to_read.size() && !this->_to_write.size()) {
      this->_max = -1;
      return (false);
   }
   if (!init)
   {
      ::bzero((void *)&timeout, sizeof(timeout));
      timeout.tv_usec = 100;
      init = true;
   }
   this->_currently_reads = this->_master_read;
   this->_currently_writes = this->_master_write;
   this->_currently_excepts = this->_master_except;
   if ((res = ::select(this->_max + 1,
                  &this->_currently_reads,
                  &this->_currently_writes,
                  &this->_currently_excepts,
                  &timeout)) == -1)
      throw UnixException();
   else if (res > 0)
   {
      for (auto it = this->_to_read.begin(); res > 0 && it != this->_to_read.end(); ++it)
      {
         if (FD_ISSET(it->first, &this->_currently_reads)) {
            --res;
            if (this->_on_read && this->_on_read(it->first, it->second)) {
               FD_CLR(it->first, &this->_master_read);
               FD_CLR(it->first, &this->_master_except);
               it = this->_to_read.erase(it);
            }
         } else if (FD_ISSET(it->first, &this->_currently_excepts)) {
            --res;
            if ((this->_on_except && this->_on_except(it->first, it->second)) || !this->_on_except) {
               FD_CLR(it->first, &this->_master_read);
               FD_CLR(it->first, &this->_master_except);
               it = this->_to_read.erase(it);
            }
         }
      }
      for (auto it = this->_to_write.begin(); res > 0 && it != this->_to_write.end(); ++it)
      {
         if (FD_ISSET(it->first, &this->_currently_writes)) {
            --res;
            if (this->_on_write && this->_on_write(it->first, it->second)) {
               FD_CLR(it->first, &this->_master_write);
               FD_CLR(it->first, &this->_master_except);
               it = this->_to_write.erase(it);
            }
         } else if (FD_ISSET(it->first, &this->_currently_excepts)) {
            --res;
            if ((this->_on_except && this->_on_except(it->first, it->second)) || !this->_on_except) {
               FD_CLR(it->first, &this->_master_write);
               FD_CLR(it->first, &this->_master_except);
               it = this->_to_write.erase(it);
            }
         }
      }
      return (true);
   }
   return (false);
}
