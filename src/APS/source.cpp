#include "APS/source.hpp"

APS::Source::Source(time_unit_t delay, const TimeManager::shared & time_manager_ptr):
  _time_manager_ptr{ time_manager_ptr },
  _subs{},
  _requests_count{ 0 },
  _delay{ delay }
{}

void APS::Source::subscribe(APS::Subscribers< Request >::function_t function)
{
  _subs.subscribe(function);
}

void APS::Source::createRequest()
{
  Request req{};
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
