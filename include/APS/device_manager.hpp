#ifndef APS_DEVICE_MANAGER_HPP
#define APS_DEVICE_MANAGER_HPP

#include <vector>
#include "APS/device.hpp"
#include "APS/buffer.hpp"

namespace APS
{
  class DeviceManager
  {
   public:
    using counter_t = APS::SharedCounter;
    using timeManager_ptr_t = APS::TimeManager::shared;
    using buffer_ptr_t = APS::Buffer::shared;
    using devices_t = std::vector< APS::Device >;

    DeviceManager(size_t size, timeManager_ptr_t time_manager_ptr, const counter_t & processed_counter, buffer_ptr_t buffer_ptr);
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

    devices_t & getDevices();

    /**
     * \brief Function that checking buffer
     */
    void check();

   private:
    devices_t _devices;
    buffer_ptr_t _buffer_ptr;
  };
}

#endif
