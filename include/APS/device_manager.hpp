#ifndef APS_DEVICE_MANAGER_HPP
#define APS_DEVICE_MANAGER_HPP

#include <vector>
#include "APS/device.hpp"

namespace APS
{
  class DeviceManager
  {
   public:
    using counter_t = APS::SharedCounter;

    DeviceManager(size_t size, APS::TimeManager::shared time_manager_ptr, const counter_t & processed_counter);
    ~DeviceManager() = default;

    /**
     * \brief Function that checks avaibility of devices
     * \returns true if any device is available
     *          false otherwise
     */
    bool isAvaible() const;

    /**
     * \throw std::logic_error if all devices are busy
     */
    void registerRequest(const Request & req) noexcept(false);

   private:
    std::vector< APS::Device > _devices;
  };
}

#endif
