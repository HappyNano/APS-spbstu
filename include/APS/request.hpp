#ifndef APS_REQUEST_HPP
#define APS_REQUEST_HPP

#include <utility>
#include "APS/time_manager.hpp"

namespace APS
{
  class Request
  {
    public:
      using this_t = Request;

      Request();
      Request(const this_t& obj) = default;
      Request(this_t&&) = default;

      this_t& operator=(const this_t& obj) = default;
      this_t& operator=(this_t&&) = default;

    private:
      APS::TimeManager::time_unit_t _create_time;

  };
}

#endif
