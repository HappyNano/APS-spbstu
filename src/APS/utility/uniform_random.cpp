#include "APS/utility/uniform_random.hpp"

APS::UniformRandom::UniformRandom(double a, double b):
  rd{},
  gen{ rd() },
  dist{ a, b }
{}

APS::UniformRandom::UniformRandom(const this_t & obj):
  rd{},
  gen{ rd() },
  dist{ obj.dist.a(), obj.dist.b() }
{}

double APS::UniformRandom::get()
{
  return dist(gen);
}
