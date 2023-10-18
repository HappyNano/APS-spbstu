#include <iostream>
#include <string>

#include "APS/engine.hpp"

#define SOURCES 3
#define BUFFER 4
#define DEVICES 2
#define ALPHA 1
#define BETA 2
#define LAMBDA 5

int main()
{
  // TODO: connect graphics
  APS::Engine engine(SOURCES, BUFFER, DEVICES, ALPHA, BETA, LAMBDA);
  engine.run();

  return 0;
}