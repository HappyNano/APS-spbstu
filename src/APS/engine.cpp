#include "APS/engine.hpp"

APS::Engine::Engine():
  _time_manager{ TimeManager::makeShared() },
  _sources{},
  _rejectReq_counter{ 0 },
  _createdReq_counter{ 0 },
  _processedReq_counter{ 0 }
{}
