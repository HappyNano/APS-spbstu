#include "APS/request.hpp"

bool APS::operator==(const Request & a, const Request & b)
{
  return a.id == b.id && a.source_id == b.source_id;
}
