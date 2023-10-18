#include <iostream>
#include <string>

#include "APS/engine.hpp"

int main()
{
  // TODO: connect graphics
  APS::Engine engine(2, 1, 2, 1, 2, 5);
  engine.run();

  return 0;
}