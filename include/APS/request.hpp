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
    int source_req_number;
    APS::TimeManager::time_unit_t create_time;
    APS::TimeManager::time_unit_t buffered_time;
    APS::TimeManager::time_unit_t registered_time;
    APS::TimeManager::time_unit_t processed_time;
    bool rejected;
    int device_id;
  };

  bool operator==(const Request &, const Request &);
}

#endif
