#include "APS/device.hpp"
#include <stdexcept>
#include <iostream>

APS::Device::Device(int id, APS::TimeManager::shared time_manager_ptr, const counter_t & processed_counter):
  _request_opt{},
  _processed_counter{ processed_counter },
  _time_manager_ptr{ time_manager_ptr },
  _subs{},
  _erand{ 1 },
  _id{ id }
{}

APS::Device::Device(this_t && obj) noexcept:
  _request_opt{ std::move(obj._request_opt) },
  _processed_counter{ std::move(obj._processed_counter) },
  _time_manager_ptr{ std::move(obj._time_manager_ptr) },
  _subs{ std::move(obj._subs) },
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
  std::swap(_subs, obj._subs);
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
  auto delay_time = _erand.get();
  // std::cout << "\033[92mDevice[" << getId() << "] Registered Request " << req.id << " from source " << req.source_id << " for time "
  //           << delay_time << " (ms)\033[0m \n";
  _time_manager_ptr->subscribeDelay(delay_time,
   [&]()
   {
     _freeDevice();
   });
  _subs.invoke();
}

const typename APS::Device::req_opt_t & APS::Device::getRequest() const
{
  return _request_opt;
}

bool APS::Device::isAvaible() const
{
  return !_request_opt.has_value();
}

void APS::Device::subscribe(const subs_func_t & function)
{
  _subs.subscribe(function);
}

void APS::Device::_freeDevice()
{
  _request_opt.value().processed_time = _time_manager_ptr->timeNow(); // Now unused
  _request_opt.reset();
  ++_processed_counter;
  _subs.invoke();
}
