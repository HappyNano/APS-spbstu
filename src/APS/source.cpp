#include "APS/source.hpp"

int APS::Source::all_request_count = 0;
int APS::Source::all_sources_count = 0;

APS::Source::Source(time_unit_t delay, const TimeManager::shared & time_manager_ptr):
  _time_manager_ptr{ time_manager_ptr },
  _subs{},
  _requests_count{ 0 },
  _delay{ delay },
  _id{ all_sources_count++ }
{
  _subscribeForTimeManager();
}

void APS::Source::subscribe(APS::Subscribers< Request >::function_t function)
{
  _subs.subscribe(function);
}

void APS::Source::createRequest()
{
  Request req{ Source::all_request_count++, _id, _time_manager_ptr->timeNow(), 0, 0 };
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
 const TimeManager::shared & time_manager_ptr)
{
  return std::make_shared< this_t >(delay, time_manager_ptr);
}
