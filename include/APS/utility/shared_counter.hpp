#ifndef APS_SHARED_COUNTED_HPP
#define APS_SHARED_COUNTED_HPP

#include <memory>

namespace APS
{
  /**
   * \brief Class SharedCounter
   */
  class SharedCounter
  {
   public:
    using this_t = SharedCounter;
    using ctype_t = int;

    SharedCounter(ctype_t start);

    operator ctype_t() const;
    ctype_t value() const;

    void incremect();
    void decrement();
    void incremect(ctype_t num);
    void decrement(ctype_t num);

    this_t & operator++();
    this_t operator++(int);
    this_t & operator--();
    this_t operator--(int);

   private:
    std::shared_ptr< ctype_t > _counter_ptr;
  };
}

#endif
