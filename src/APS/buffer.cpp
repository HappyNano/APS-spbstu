#include "APS/buffer.hpp"
#include <stdexcept>
#include <algorithm>

APS::Buffer::Buffer(size_t size):
  _req_memory{}
{
  _req_memory.reserve(size);
}

bool APS::Buffer::is_avaible() const
{
  return _req_memory.size() < _req_memory.capacity();
}

bool APS::Buffer::is_empty() const
{
  return _req_memory.empty();
}

APS::Request APS::Buffer::getRequestByPriorityThrow() noexcept(false)
{
  auto value = getRequestByPriority();
  if (value)
  {
    throw std::length_error("Buffer is empty");
  }
  return *value;
}

std::optional< APS::Request > APS::Buffer::getRequestByPriority() noexcept(true)
{
  if (is_empty())
  {
    return std::nullopt;
  }

  auto highest_priority_it = std::min_element(_req_memory.begin(),
   _req_memory.end(),
   [](const auto & lhs, const auto & rhs)
   {
     if (lhs.second.source_id != rhs.second.source_id)
     {
       return lhs.second.source_id < rhs.second.source_id;
     }

     return lhs.first < rhs.first;
   });

  APS::Request highest_priority_request = highest_priority_it->second;
  _req_memory.erase(highest_priority_it);

  return highest_priority_request;
}

void APS::Buffer::registerRequest(const Request & req)
{}

// Static function
typename APS::Buffer::shared APS::Buffer::makeShared(size_t size)
{
  return std::make_shared< Buffer >(size);
}
