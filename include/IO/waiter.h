#ifndef HS_IO_WAITER_H
# define HS_IO_WAITER_H

# include <sys/types.h>
# include <list>

namespace hs
{
   namespace io
   {
      class Waiter
      {
         typedef bool (*handler_event)(int, void *);
      private:
         std::list<std::pair<int, void*>>             _to_read;
         std::list<std::pair<int, void*>>             _to_write;
         handler_event                                _on_read = nullptr;
         handler_event                                _on_write = nullptr;
         handler_event                                _on_except = nullptr;
         fd_set   _master_read;
         fd_set   _master_write;
         fd_set   _master_except;
         fd_set   _currently_reads;
         fd_set   _currently_writes;
         fd_set   _currently_excepts;
         int      _max;

      public:
         explicit Waiter() noexcept;
         virtual  ~Waiter() noexcept;

         void  add(bool toread, int fd, void *data = nullptr);
         bool  processEvents();

         inline void setOnRead (handler_event on_read) {
            this->_on_read = on_read;
         }

         inline void setOnWrite (handler_event on_write) {
            this->_on_write = on_write;
         }

         inline void setOnExcept (handler_event on_except) {
            this->_on_except = on_except;
         }
      };
   }
}

#endif
