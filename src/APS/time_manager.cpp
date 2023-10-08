#include "APS/time_manager.hpp"

APS::TimeManager::TimeManager():
  _future_event{},
  _time_now_ms{ 0 }
{}

APS::TimeManager::TimeManager(time_unit_t start_time):
  TimeManager()
{
  _time_now_ms = start_time;
}

void APS::TimeManager::subscribePoint(time_unit_t time_point, const function_t & function)
{
  _future_event[time_point].subscribe(function);
}

void APS::TimeManager::subscribeDelay(time_unit_t time_delay, const function_t & function)
{
  this->subscribePoint(_time_now_ms + time_delay, function);
}

void APS::TimeManager::stepToEvent()
{
  // wait for special event
  for (auto && event: _future_event)
  {
    if (event.first < _time_now_ms)
    {
      break;
    }
    event.second.invoke();
  }
}

typename APS::TimeManager::time_unit_t APS::TimeManager::timeNow() const
{
  return _time_now_ms;
}