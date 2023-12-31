#ifndef APS_SOURCE_HPP
#define APS_SOURCE_HPP

#include <memory>
#include "APS/time_manager.hpp"
#include "APS/request.hpp"
#include "APS/utility/shared_counter.hpp"
#include "APS/utility/uniform_random.hpp"

namespace APS
{
  /**
   * \brief Source - Generator of Request
   */
  class Source
  {
   public:
    using this_t = Source;
    using shared = std::shared_ptr< Source >;
    using time_unit_t = TimeManager::time_unit_t;
    using counter_t = SharedCounter;

    Source(time_unit_t a, time_unit_t b, int id, const counter_t & req_counter, const TimeManager::shared & time_manager_);
    Source(const this_t & obj) = delete;
    Source(this_t &&) = delete;

    this_t & operator=(const this_t & obj) = delete;
    this_t & operator=(this_t &&) noexcept = delete;

    /**
     * \brief Function to subsribe for creating requests
     * \param function function
     */
    void subscribe(APS::Subscribers< Request >::function_t function);
    void createRequest();

    static Source::shared makeShared(time_unit_t a,
     time_unit_t b,
     int id,
     counter_t req_counter,
     const TimeManager::shared & time_manager_ptr);

    inline static bool stop = false;

   private:
    TimeManager::shared _time_manager_ptr;

    APS::Subscribers< Request > _subs;
    __int16_t _requests_count; // Number of requests maked by this source
    time_unit_t _a;
    time_unit_t _b;
    APS::UniformRandom _urand;
    counter_t _requests_counter;
    int _id;

    /**
     * \brief Subscribe to a future request after a delay time
     */
    void _subscribeForTimeManager();
  };
}

#endif
