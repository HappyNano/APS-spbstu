#include "APS/request_manager.hpp"
#include <iostream>

APS::RequestManager::RequestManager(APS::Buffer::shared buffer_ptr, const counter_t & reject_counter):
  _buffer_ptr{ buffer_ptr },
  _reject_counter{ reject_counter },
  _subs_reject{}
{}

void APS::RequestManager::registerRequest(const Request & req)
{
  if (!_buffer_ptr->is_avaible())
  {
    ++_reject_counter;
    auto tmp_req = req;
    tmp_req.rejected = true;
    _subs_reject.invoke(tmp_req);
    return;
  }
  _buffer_ptr->registerRequest(req);
}

void APS::RequestManager::subscribeReject(const APS::Subscribers< Request >::function_t & function)
{
  _subs_reject.subscribe(function);
}
