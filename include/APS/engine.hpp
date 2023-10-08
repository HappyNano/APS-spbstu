#ifndef APS_ENGINE_HPP
#define APS_ENGINE_HPP

#include "APS/source.hpp"
#include "APS/time_manager.hpp"

namespace APS
{
  class Engine
  {
   public:
    Engine();

   private:
    TimeManager::shared _time_manager;
    std::vector< Source::shared > _sources;
  };
}

#endif
