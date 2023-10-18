#ifndef APS_UTILITY_UNIFORM_RANDOM_HPP
#define APS_UTILITY_UNIFORM_RANDOM_HPP

#include <random>

namespace APS
{
  class UniformRandom
  {
   public:
    using this_t = UniformRandom;

    UniformRandom(double a, double b);
    UniformRandom(const this_t &);

    double get();

   private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> dist;
  };
}

#endif
