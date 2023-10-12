#include "APS/utility/shared_counter.hpp"

APS::SharedCounter::SharedCounter(ctype_t start):
  _counter_ptr{ std::make_shared< ctype_t >(start) }
{}

APS::SharedCounter::operator ctype_t() const
{
  return value();
}

typename APS::SharedCounter::ctype_t APS::SharedCounter::value() const
{
  return *_counter_ptr;
}

void APS::SharedCounter::incremect()
{
  ++(*_counter_ptr);
}

void APS::SharedCounter::decrement()
{
  --(*_counter_ptr);
}

void APS::SharedCounter::incremect(ctype_t num)
{
  *_counter_ptr += num;
}

void APS::SharedCounter::decrement(ctype_t num)
{
  *_counter_ptr -= num;
}

typename APS::SharedCounter::this_t & APS::SharedCounter::operator++()
{
  incremect();
  return *this;
}

typename APS::SharedCounter::this_t APS::SharedCounter::operator++(int)
{
  this_t tmp = *this;
  ++(*this);
  return tmp;
}

typename APS::SharedCounter::this_t & APS::SharedCounter::operator--()
{
  decrement();
  return *this;
}

typename APS::SharedCounter::this_t APS::SharedCounter::operator--(int)
{
  this_t tmp = *this;
  --(*this);
  return tmp;
}
