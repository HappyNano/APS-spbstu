#ifndef APs_REQUEST_MANAGER_HPP
#define APs_REQUEST_MANAGER_HPP

#include "APS/source.hpp"
#include "APS/buffer.hpp"
#include "APS/utility/shared_counter.hpp"

namespace APS
{
  class RequestManager
  {
   public:
    using this_t = RequestManager;
    using counter_t = APS::SharedCounter;

    RequestManager(APS::Buffer::shared buffer_ptr, const counter_t & reject_counter);
    RequestManager(const this_t & obj) = delete;
    RequestManager(this_t &&) = delete;

    this_t & operator=(const this_t & obj) = delete;
    this_t & operator=(this_t &&) = delete;

    /**
     * \brief Function of registering incoming request
     *        and transferring them further to the buffer if it is available
     * \param req is incoming request
     */
    void registerRequest(const Request & req);

   private:
    APS::Buffer::shared _buffer_ptr;
    counter_t _reject_counter;
  };
}

#endif
