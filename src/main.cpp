#include <iostream>
#include <string>

#include "APS/utility/subscribers.hpp"
#include "APS/time_manager.hpp"
#include "APS/device_manager.hpp"

int main()
{
  APS::TimeManager::shared tm_ptr = APS::TimeManager::makeShared();
  APS::SharedCounter cntr(0);
  APS::Device dev(tm_ptr, cntr);

  return 0;
}