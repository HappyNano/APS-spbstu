#include "APS/engine.hpp"

APS::Engine::Engine():
  _time_manager{ TimeManager::makeShared() },
  _sources{}
{}