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
       std::cout << "\033[94mSource[" << i << "] generated request in time " << this->_time_manager_ptr->timeNow() << " \t(ms)\033[0m\n";
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
     [&device]()
     {
       std::cout << "Device[" << device.getId() << "] Released\n";
     });
  }
}

void APS::Engine::run()
{
  while (_time_manager_ptr->timeNow() < 10)
  {
    _time_manager_ptr->stepToEvent();
  }

  std::cout << '\n';
  std::cout << "Created: " << _createdReq_counter.value() << '\n';
  std::cout << "Processed: " << _processedReq_counter.value() << '\n';
  std::cout << "Rejected: " << _rejectReq_counter.value() << '\n';
}
