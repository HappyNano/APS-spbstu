#include "APS/request_manager.hpp"
#include <iostream>

APS::RequestManager::RequestManager(APS::Buffer::shared buffer_ptr, const counter_t & reject_counter):
  _buffer_ptr{ buffer_ptr },
  _reject_counter{ reject_counter }
{}

void APS::RequestManager::registerRequest(const Request & req)
{
  if (!_buffer_ptr->is_avaible())
  {
    // Rejection
    ++_reject_counter;
    std::cout << "\033[91mRequest " << req.id << " rejected\033[0m\n";
    return;
  }
  _buffer_ptr->registerRequest(req);
}
