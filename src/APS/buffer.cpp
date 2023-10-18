#include "APS/buffer.hpp"
#include <stdexcept>
#include <algorithm>
#include <numeric>

APS::Buffer::Buffer(size_t size, const time_manager_ptr_t & time_manager_ptr):
  _time_manager_ptr{ time_manager_ptr },
  _req_memory{},
  _subs{}
{
  _req_memory.resize(size);
}

bool APS::Buffer::is_avaible() const
{
  bool any_has_not_value = std::any_of(_req_memory.begin(),
   _req_memory.end(),
   [](const std::optional< vec_elem_t > & el)
   {
     return !el.has_value();
   });
  return any_has_not_value;
}

bool APS::Buffer::is_empty() const
{
  bool any_has_value = std::any_of(_req_memory.begin(),
   _req_memory.end(),
   [](const std::optional< vec_elem_t > & el)
   {
     return el.has_value();
   });
  return !any_has_value;
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

  auto valid_requests = std::accumulate(_req_memory.begin(),
   _req_memory.end(),
   std::vector< vec_elem_t >(),
   [](auto & acc, const std::optional< vec_elem_t > & elem)
   {
     if (elem.has_value())
     {
       acc.push_back(elem.value());
     }
     return acc;
   });

  auto highest_priority_it = std::min_element(valid_requests.begin(),
   valid_requests.end(),
   [](const vec_elem_t & lhs, const vec_elem_t & rhs)
   {
     if (lhs.second.source_id != rhs.second.source_id)
     {
       return lhs.second.source_id < rhs.second.source_id;
     }

     return lhs.first < rhs.first;
   });

  APS::Request highest_priority_request = highest_priority_it->second;
  auto _req_memory_iter_toErase = std::find_if(_req_memory.begin(),
   _req_memory.end(),
   [&](const std::optional< vec_elem_t > & el)
   {
     return el.has_value() && el.value().second == highest_priority_request;
   });
  _req_memory_iter_toErase->reset();

  return highest_priority_request;
}

void APS::Buffer::registerRequest(const Request & req)
{
  if (!is_avaible())
  {
    return;
  }

  auto _req_memory_iter_toInsert = std::find_if(_req_memory.begin(),
   _req_memory.end(),
   [](const std::optional< vec_elem_t > & el)
   {
     return !el.has_value();
   });

  auto req_tmp = req;
  req_tmp.registered_time = _time_manager_ptr->timeNow();
  _req_memory_iter_toInsert->emplace(_time_manager_ptr->timeNow(), req_tmp);
  _subs.invoke();
}

void APS::Buffer::subscribe(const APS::Subscribers<>::function_t & function)
{
  _subs.subscribe(function);
}

const typename APS::Buffer::vec_type & APS::Buffer::getBuffer() const
{
  return _req_memory;
}

// Static function
typename APS::Buffer::shared APS::Buffer::makeShared(size_t size, const time_manager_ptr_t & time_manager_ptr)
{
  return std::make_shared< Buffer >(size, time_manager_ptr);
}
