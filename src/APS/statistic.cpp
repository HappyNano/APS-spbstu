#include "APS/statistic.hpp"
#include <set>
#include <iostream>

APS::Statistic::Statistic(const std::vector< Request > & requests):
  _count_createdReq{ 0 },
  _count_processedReq{ 0 },
  _count_rejectedReq{ 0 },
  _sources{},
  _devices{}
{
  std::set< int > sources_id;
  std::set< int > devices_id;
  for (auto && req: requests)
  {
    ++_count_createdReq;
    sources_id.insert(req.source_id);
    if (req.rejected)
    {
      ++_count_rejectedReq;
    }
    else
    {
      devices_id.insert(req.device_id);
      ++_count_processedReq;
    }
  }
  for (auto source_id: sources_id)
  {
    _sources.emplace(source_id, APS::SourceStatistic(source_id, requests));
  }
  for (auto device_id: devices_id)
  {
    _devices.emplace(device_id, APS::DeviceStatistic(device_id, requests));
  }
}

size_t APS::Statistic::count_createdReq() const
{
  return _count_createdReq;
}
size_t APS::Statistic::count_processedReq() const
{
  return _count_processedReq;
}
size_t APS::Statistic::count_rejectedReq() const
{
  return _count_rejectedReq;
}
const std::map< int, APS::SourceStatistic > & APS::Statistic::sources() const
{
  return _sources;
}
const std::map< int, APS::DeviceStatistic > & APS::Statistic::devices() const
{
  return _devices;
}

APS::SourceStatistic::SourceStatistic(int source_id, const std::vector< Request > & requests):
  _source_id{ source_id },
  _count_createdReq{ 0 },
  _p_rejected{ 0 },
  _average_T_in{ 0 },
  _average_T_buffered{ 0 },
  _average_T_service{ 0 }
{
  size_t count_processed = 0;
  size_t count_rejected = 0;
  for (auto && req: requests)
  {
    if (req.source_id != _source_id)
    {
      continue;
    }
    ++_count_createdReq;

    if (req.rejected)
    {
      ++count_rejected;
    }
    else
    {
      ++count_processed;
      _average_T_in += req.processed_time - req.create_time;
      _average_T_buffered += req.registered_time - req.buffered_time;
      _average_T_service += req.processed_time - req.registered_time;
    }
  }
  _p_rejected = (1.f * count_rejected) / _count_createdReq;

  _average_T_in /= count_processed;
  _average_T_buffered /= count_processed;
  _average_T_service /= count_processed;
}

int APS::SourceStatistic::id() const
{
  return _source_id;
}
size_t APS::SourceStatistic::countCreated() const
{
  return _count_createdReq;
}
double APS::SourceStatistic::p_rejected() const
{
  return _p_rejected;
}
double APS::SourceStatistic::average_TimeIn() const
{
  return _average_T_in;
}
double APS::SourceStatistic::average_TimeBuffered() const
{
  return _average_T_buffered;
}
double APS::SourceStatistic::average_TimeService() const
{
  return _average_T_service;
}

APS::DeviceStatistic::DeviceStatistic(int device_id, const std::vector< Request > & requests):
  _device_id{ device_id },
  _using_coef{ 0 }
{
  double realization_time = requests.back().processed_time - requests.front().create_time;
  for (auto && req: requests)
  {
    if (req.rejected || req.device_id != _device_id)
    {
      continue;
    }
    _using_coef += req.processed_time - req.registered_time;
  }
  _using_coef /= realization_time;
}

int APS::DeviceStatistic::id() const
{
  return _device_id;
}
double APS::DeviceStatistic::usingCoef() const
{
  return _using_coef;
}
