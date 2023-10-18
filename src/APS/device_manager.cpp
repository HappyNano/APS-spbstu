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
    _devices.emplace_back(time_manager_ptr, processed_counter);
  }
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
