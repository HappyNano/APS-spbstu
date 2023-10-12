#include "APS/source.hpp"

APS::Source::Source(time_unit_t delay,
 int id,
 counter_t req_counter,
 const TimeManager::shared & time_manager_ptr):
  _time_manager_ptr{ time_manager_ptr },
  _subs{},
  _requests_count{ 0 },
  _delay{ delay },
  _requests_counter{ req_counter },
  _id{ id }
{
  _subscribeForTimeManager();
}

void APS::Source::subscribe(APS::Subscribers< Request >::function_t function)
{
  _subs.subscribe(function);
}

void APS::Source::createRequest()
{
  Request req{ _requests_counter++, _id, _time_manager_ptr->timeNow(), 0, 0 };
  ++_requests_count;

  _subscribeForTimeManager();

  _subs.invoke(req);
}

void APS::Source::_subscribeForTimeManager()
{
  _time_manager_ptr->subscribeDelay(_delay,
   [&]()
   {
     this->createRequest();
   });
}

// Static function
APS::Source::shared APS::Source::makeShared(time_unit_t delay,
 int id,
 counter_t req_counter,
 const TimeManager::shared & time_manager_ptr)
{
  return std::make_shared< this_t >(delay, id, req_counter, time_manager_ptr);
}
