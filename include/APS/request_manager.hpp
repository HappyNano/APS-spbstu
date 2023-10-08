#ifndef APs_REQUEST_MANAGER_HPP
#define APs_REQUEST_MANAGER_HPP

#include "APS/source.hpp"

namespace APS
{
  class RequestManager
  {
    public:
      using this_t = RequestManager;

      RequestManager() = default;
      RequestManager(const this_t& obj) = delete;
      RequestManager(this_t&&) = delete;

      this_t& operator=(const this_t& obj) = delete;
      this_t& operator=(this_t&&) = delete;
      

    private:

  };
}

#endif
