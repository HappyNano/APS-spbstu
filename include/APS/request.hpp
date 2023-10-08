#ifndef APS_REQUEST_HPP
#define APS_REQUEST_HPP

#include <utility>
#include "APS/time_manager.hpp"

namespace APS
{
  struct Request
  {
    int id;
    int source_id;
    APS::TimeManager::time_unit_t create_time;
    APS::TimeManager::time_unit_t registered_time;
    APS::TimeManager::time_unit_t processed_time;
  };
}

#endif
