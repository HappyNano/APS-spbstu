#include "APS/source.hpp"

APS::Source::Source(time_unit_t a, time_unit_t b, int id, const counter_t & req_counter, const TimeManager::shared & time_manager_ptr):
  _time_manager_ptr{ time_manager_ptr },
  _subs{},
  _requests_count{ 0 },
  _a{ a },
  _b{ b },
  _urand{ a, b },
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
  if (Source::stop)
  {
    return;
  }

  Request req{ _requests_counter++, _id, _requests_count++, _time_manager_ptr->timeNow(), 0, 0, 0, false, -1 };
  ++_requests_count;

  _subscribeForTimeManager();

  _subs.invoke(req);
}

void APS::Source::_subscribeForTimeManager()
{
  _time_manager_ptr->subscribeDelay(_urand.get(),
   [&]()
   {
     this->createRequest();
   });
}

// Static function
APS::Source::shared APS::Source::makeShared(time_unit_t a,
 time_unit_t b,
 int id,
 counter_t req_counter,
 const TimeManager::shared & time_manager_ptr)
{
  return std::make_shared< this_t >(a, b, id, req_counter, time_manager_ptr);
}
