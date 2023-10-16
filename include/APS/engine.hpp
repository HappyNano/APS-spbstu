#ifndef APS_ENGINE_HPP
#define APS_ENGINE_HPP

#include "APS/source.hpp"
#include "APS/time_manager.hpp"
#include "APS/request_manager.hpp"

namespace APS
{
  class Engine
  {
   public:
    Engine();
    // Engine(int sources_count)

   private:
    APS::TimeManager::shared _time_manager;
    std::vector< Source::shared > _sources;

    // Counters
    APS::SharedCounter _rejectReq_counter;
    APS::SharedCounter _createdReq_counter;
    APS::SharedCounter _processedReq_counter;
  };
}

#endif
