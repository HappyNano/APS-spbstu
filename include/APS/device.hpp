#ifndef APS_DEVICE_HPP
#define APS_DEVICE_HPP

#include <optional>

#include "APS/request.hpp"
#include "APS/time_manager.hpp"
#include "APS/utility/shared_counter.hpp"
#include "APS/utility/exponential_random.hpp"

namespace APS
{
  class Device
  {
   public:
    using this_t = Device;
    using counter_t = APS::SharedCounter;
    using req_opt_t = std::optional< Request >;
    using subs_func_t = APS::Subscribers< Request >::function_t;

    Device(int id, APS::TimeManager::shared time_manager_ptr, const counter_t & processed_counter, double lambda);
    Device(const this_t &) = default;
    Device(this_t &&) noexcept;
    ~Device() = default;

    this_t & operator=(const this_t &) = default;
    this_t & operator=(this_t &&) noexcept;
    void swap(this_t &) noexcept;

    int getId() const;

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
     * \note Connect here "check" method of APS::DeviceManager
     * \param function function
     */
    void subscribeRelease(const subs_func_t & function);

    /**
     * \brief Function to subsribe for device register request
     * \param function function
     */
    void subscribeRegistered(const subs_func_t & function);

   private:
    std::optional< Request > _request_opt;
    APS::SharedCounter _processed_counter;
    APS::TimeManager::shared _time_manager_ptr;
    APS::Subscribers< Request > _subs_registered;
    APS::Subscribers< Request > _subs_release;
    APS::ExponentialRandom _erand;
    int _id;

    void _freeDevice();
  };
}

#endif
