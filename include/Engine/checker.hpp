#ifndef HS_ENGINE_CHECKER_HPP
# define HS_ENGINE_CHECKER_HPP

namespace hs
{
   namespace engine
   {
      template <class Checker1, class Checker2>
      struct CheckOr
      {
         static inline bool check(const char *buffer, size_t& pos, size_t len) noexcept {
            return (Checker1::check(buffer, pos, len) || Checker2::check(buffer, pos, len));
         }
      };
      template <class Checker1, class Checker2>
      struct CheckAnd
      {
         static inline bool check(const char *buffer, size_t& pos, size_t len) noexcept {
            return (Checker1::check(buffer, pos, len) && Checker2::check(buffer, pos, len));
         }
      };
   }
}

#endif
