#include <iostream>
#include <string>

#include "APS/utility/subscribers.hpp"
#include "APS/time_manager.hpp"
#include "APS/device_manager.hpp"

int main()
{
  // TODO: connect graphics
  // TODO: create Engine
  APS::TimeManager::shared tm_ptr = APS::TimeManager::makeShared();
  APS::SharedCounter cntr(0);
  APS::Device dev(tm_ptr, cntr);

  std::vector< int > vec;
  vec.push_back(10);
  vec.push_back(11);
  vec.push_back(12);
  vec.push_back(13);
  vec.push_back(14);
  vec.shrink_to_fit();

  for (auto && el: vec)
  {
    std::cout << el << ' ';
  }
  std::cout << "Size = " << vec.size() << " capacity = " << vec.capacity() << '\n';

  vec.erase(std::begin(vec) + 2);

  for (auto && el: vec)
  {
    std::cout << el << ' ';
  }
  std::cout << "Size = " << vec.size() << " capacity = " << vec.capacity() << '\n';

  std::cout << vec[1] << '\n';

  return 0;
}