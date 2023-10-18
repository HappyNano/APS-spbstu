#include "APS/utility/exponential_random.hpp"

APS::ExponentialRandom::ExponentialRandom(double lambda):
  rd{},
  gen{ rd() },
  dist{ lambda }
{}

APS::ExponentialRandom::ExponentialRandom(const this_t & obj):
  rd{},
  gen{ rd() },
  dist{ obj.dist.lambda() }
{}

double APS::ExponentialRandom::get()
{
  return dist(gen);
}
