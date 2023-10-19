#include "APS/engine.hpp"
#include <iostream>

APS::Engine::Engine(size_t sources_count, size_t buffer_size, size_t devices_count, double alpha, double beta, double lambda):
  _rejectReq_counter{ 0 },
  _createdReq_counter{ 0 },
  _processedReq_counter{ 0 },
  _time_manager_ptr{ TimeManager::makeShared() },
  _sources{},
  _buffer_ptr{ Buffer::makeShared(buffer_size, _time_manager_ptr) },
  _req_manager{ _buffer_ptr, _rejectReq_counter },
  _device_manager{ devices_count, _time_manager_ptr, _processedReq_counter, _buffer_ptr }
{
  for (int i = 0; i < sources_count; ++i)
  {
    _sources.emplace_back(Source::makeShared(alpha, beta, i, _createdReq_counter, _time_manager_ptr));
    _sources.back()->subscribe(
     [this, i](const Request & req)
     {
       //  std::cout << "\033[94mSource[" << i << "] generated request in time " << this->_time_manager_ptr->timeNow() << "
       //  \t(ms)\033[0m\n";
       printState(i);
       this->_req_manager.registerRequest(req);
     });
  }

  // Linking
  _buffer_ptr->subscribe(
   [this]()
   {
     this->_device_manager.check();
   });

  for (auto && device: _device_manager.getDevices())
  {
    device.subscribe(
     [this, &device]()
     {
       //  if (device.isAvaible())
       //  {
       //    std::string res;
       //    res += "\033[92mDevice[" + std::to_string(device.getId()) + "] released ";
       //    res += "\033[0m ";
       //    std::cout << res << std::endl;
       //  }
       //  std::cout << "Device[" << device.getId() << "] Released\n";
       printState();
     });
  }
}

void APS::Engine::printState(int from_source)
{
  static TimeManager::time_unit_t last_time = 0;
  std::string res;
  // if (from_source != -1)
  // {
  //   res += "\033[92mNew request from source ";
  //   res += std::to_string(from_source);
  //   res += "\033[0m ";
  //   std::cout << res << std::endl;
  //   return;
  // }
  res += "Time ";
  res += std::to_string(_time_manager_ptr->timeNow());
  res += " (ms) \t";

  res += " Sources: ";
  int i = 0;
  for (auto && source: _sources)
  {
    if (i == from_source)
    {
      res += "\033[92m";
      res += "[" + std::to_string(i) + "]";
    }
    else
    {
      res += "\033[94m";
      res += "[-]";
    }
    res += "\033[0m ";
    ++i;
  }

  res += " Buffer: ";
  for (auto && buf_item: _buffer_ptr->getBuffer())
  {
    if (buf_item.has_value())
    {
      res += "\033[91m";
      res += "[" + std::to_string(buf_item->second.source_id) + "]";
    }
    else
    {
      res += "\033[92m";
      res += "[-]";
    }
    res += "\033[0m ";
  }

  res += "Devices: ";
  for (auto && device: _device_manager.getDevices())
  {
    if (device.isAvaible())
    {
      res += "\033[94m";
      res += "[-]";
    }
    else
    {
      res += "\033[91m";
      res += "[" + std::to_string(device.getRequest().value().source_id) + "]";
    }
    res += "\033[0m ";
  }
  res += "  " + std::to_string(_time_manager_ptr->timeNow() - last_time) + " (ms) ";
  last_time = _time_manager_ptr->timeNow();
  std::cout << res << std::endl;
}

void APS::Engine::run()
{
  while (_createdReq_counter.value() < 100)
  {
    _time_manager_ptr->stepToEvent();
    // std::cin.get();
  }

  std::cout << '\n';
  std::cout << "Created: " << _createdReq_counter.value() << '\n';
  std::cout << "Processed: " << _processedReq_counter.value() << '\n';
  std::cout << "Rejected: " << _rejectReq_counter.value() << '\n';
}
