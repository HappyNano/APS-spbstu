#ifndef APS_DEVICE_HPP
#define APS_DEVICE_HPP

#include <optional>

#include "APS/request.hpp"
#include "APS/time_manager.hpp"
#include "APS/utility/shared_counter.hpp"

namespace APS
{
  class Device
  {
   public:
    using this_t = Device;
    using counter_t = APS::SharedCounter;
    using req_opt_t = std::optional< Request >;
    using subs_func_t = APS::Subscribers<>::function_t;

    Device(APS::TimeManager::shared time_manager_ptr, const counter_t & processed_counter);
    Device(const this_t &) = default;
    Device(this_t &&) noexcept;
    ~Device() = default;

    this_t & operator=(const this_t &) = default;
    this_t & operator=(this_t &&) noexcept;
    void swap(this_t &) noexcept;

    /**
     * \throw std::logic_error if device busy
     */
    void registerRequest(const Request & req) noexcept(false);

    /**
     * \returns std::optional< Request >
     */
    const req_opt_t & getRequest() const;

    /**
     * \brief Functions that checks avaibility of device
     * \returns true if avaible
     *          false otherwise
     */
    bool isAvaible() const;

    /**
     * \brief Function to subsribe for device release
     * \note Connect here checkBuffer method of APS::Buffer
     * \param function function
     */
    void subscribe(const subs_func_t & function);

   private:
    std::optional< Request > _request_opt;
    APS::SharedCounter _processed_counter;
    APS::TimeManager::shared _time_manager_ptr;
    APS::Subscribers<> _subs;

    void _freeDevice();
  };
}

#endif