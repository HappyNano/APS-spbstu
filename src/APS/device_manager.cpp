#include "APS/device_manager.hpp"
#include <algorithm>
#include <stdexcept>
#include <functional>

APS::DeviceManager::DeviceManager(size_t size,
 timeManager_ptr_t time_manager_ptr,
 const counter_t & processed_counter,
 buffer_ptr_t buffer_ptr):
  _devices{},
  _buffer_ptr{ buffer_ptr }
{
  for (size_t i = 0; i < size; ++i)
  {
    _devices.emplace_back(i, time_manager_ptr, processed_counter);
    _devices.back().subscribeRelease(
     [this](const Request &)
     {
       this->check();
     });
  }
  _buffer_ptr->subscribeRegistered(
   [this](const Request &)
   {
     this->check();
   });
}

bool APS::DeviceManager::isAvaible() const
{
  auto check_device_avaibility = std::bind(&Device::isAvaible, std::placeholders::_1);
  return std::any_of(std::cbegin(_devices), std::cend(_devices), check_device_avaibility);
}

void APS::DeviceManager::registerRequest(const Request & req) noexcept(false)
{
  if (!isAvaible())
  {
    throw std::logic_error("All devices are busy");
  }

  auto check_device_avaibility = std::bind(&Device::isAvaible, std::placeholders::_1);
  auto device_iter = std::find_if(std::begin(_devices), std::end(_devices), check_device_avaibility);
  device_iter->registerRequest(req);
}

typename APS::DeviceManager::devices_t & APS::DeviceManager::getDevices()
{
  return _devices;
}

void APS::DeviceManager::check()
{
  if (!isAvaible() || _buffer_ptr->is_empty())
  {
    return;
  }

  registerRequest(_buffer_ptr->getRequestByPriority().value());
}
