#include "APS/device.hpp"
#include <stdexcept>
#include <iostream>

APS::Device::Device(int id, APS::TimeManager::shared time_manager_ptr, const counter_t & processed_counter):
  _request_opt{},
  _processed_counter{ processed_counter },
  _time_manager_ptr{ time_manager_ptr },
  _subs_registered{},
  _subs_release{},
  _erand{ 1 },
  _id{ id }
{}

APS::Device::Device(this_t && obj) noexcept:
  _request_opt{ std::move(obj._request_opt) },
  _processed_counter{ std::move(obj._processed_counter) },
  _time_manager_ptr{ std::move(obj._time_manager_ptr) },
  _subs_registered{ std::move(obj._subs_registered) },
  _subs_release{ std::move(obj._subs_release) },
  _erand{ obj._erand }
{}

typename APS::Device::this_t & APS::Device::operator=(this_t && obj) noexcept
{
  if (this != std::addressof(obj))
  {
    this_t tmp(std::move(obj));
    swap(obj);
  }
  return *this;
}

void APS::Device::swap(this_t & obj) noexcept
{
  std::swap(_request_opt, obj._request_opt);
  std::swap(_processed_counter, obj._processed_counter);
  std::swap(_time_manager_ptr, obj._time_manager_ptr);
  std::swap(_subs_registered, obj._subs_registered);
  std::swap(_subs_release, obj._subs_release);
}

int APS::Device::getId() const
{
  return _id;
}

void APS::Device::registerRequest(const Request & req) noexcept(false)
{
  if (!isAvaible())
  {
    throw std::logic_error("Device has already request");
  }
  _request_opt.emplace(req);
  _request_opt.value().registered_time = _time_manager_ptr->timeNow();

  auto delay_time = _erand.get();
  _time_manager_ptr->subscribeDelay(delay_time,
   [&]()
   {
     _freeDevice();
   });
  _subs_registered.invoke(_request_opt.value());
}

const typename APS::Device::req_opt_t & APS::Device::getRequest() const
{
  return _request_opt;
}

bool APS::Device::isAvaible() const
{
  return !_request_opt.has_value();
}

void APS::Device::subscribeRelease(const subs_func_t & function)
{
  _subs_release.subscribe(function);
}

void APS::Device::subscribeRegistered(const subs_func_t & function)
{
  _subs_registered.subscribe(function);
}

void APS::Device::_freeDevice()
{
  Request req = _request_opt.value();
  req.processed_time = _time_manager_ptr->timeNow(); // Now unused
  _request_opt.reset();
  ++_processed_counter;
  _subs_release.invoke(req);
}
