#ifndef APS_TIME_MANAGER_HPP
#define APS_TIME_MANAGER_HPP

#include <memory>
#include "APS/utility/subscribers.hpp"

#include <map>
#include <vector>

namespace APS
{
  /**
   * \brief TimeManager
   */
  class TimeManager
  {
   public:
    using this_t = TimeManager;
    using time_unit_t = __int128_t;
    using shared = std::shared_ptr< TimeManager >;
    using function_t = APS::Subscribers<>::function_t;

    TimeManager();
    explicit TimeManager(time_unit_t start_time);
    TimeManager(const this_t & obj) = delete;
    TimeManager(this_t &&) = delete;

    this_t & operator=(const this_t & obj) = delete;
    this_t & operator=(this_t &&) = delete;

    /**
     * \brief Method to call function in time point
     * \param time_point is time, when to invoke the function
     * \param function is function to invoke
     */
    void subscribePoint(time_unit_t time_point, const function_t & function);
    /**
     * \brief Method to call a function after time delay
     * \param time_delay is time after which the function should be called
     * \param function is function to invoke
     */
    void subscribeDelay(time_unit_t time_delay, const function_t & function);

    void stepToEvent();
    /**
     * \return time now in ms
     */
    time_unit_t timeNow() const;

    template < typename... Args >
    static shared makeShared(Args &&... args);

   private:
    std::map< time_unit_t, APS::Subscribers<> > _future_event;
    time_unit_t _time_now_ms; // Time now (ms)
  };
}

// Static function
template < typename... Args >
typename APS::TimeManager::shared APS::TimeManager::makeShared(Args &&... args)
{
  return std::make_shared< TimeManager >(std::forward< Args >(args)...);
}

#endif
