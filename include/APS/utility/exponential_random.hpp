#ifndef APS_UTILITY_EXPONENTIAL_RANDOM_HPP
#define APS_UTILITY_EXPONENTIAL_RANDOM_HPP

#include <random>

namespace APS
{
  class ExponentialRandom
  {
   public:
    using this_t = ExponentialRandom;

    ExponentialRandom(double lambda);
    ExponentialRandom(const this_t &);

    double get();

   private:
    std::random_device rd;
    std::mt19937 gen;
    std::exponential_distribution<> dist;
  };
}

#endif
