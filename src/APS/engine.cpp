#include "APS/engine.hpp"
#include <iostream>

#define RED "\033[91m"
#define GREEN "\033[92m"
#define BLUE "\033[94m"
#define DEFAULT "\033[0m"

APS::Engine::Engine(size_t sources_count, size_t buffer_size, size_t devices_count, double alpha, double beta, double lambda):
  _rejectReq_counter{ 0 },
  _createdReq_counter{ 0 },
  _processedReq_counter{ 0 },
  _time_manager_ptr{ TimeManager::makeShared() },
  _sources{},
  _buffer_ptr{ Buffer::makeShared(buffer_size, _time_manager_ptr) },
  _req_manager{ _buffer_ptr, _rejectReq_counter },
  _device_manager{ devices_count, _time_manager_ptr, _processedReq_counter, _buffer_ptr },
  _update_subs{}
{
  for (int i = 0; i < sources_count; ++i)
  {
    _sources.emplace_back(Source::makeShared(alpha, beta, i, _createdReq_counter, _time_manager_ptr));
    // Link functions when source generates request
    _sources.back()->subscribe(
     [this, i](const Request & req)
     {
       _update_subs.invoke(EngineEvent::SourceGenerated, req);
       this->_req_manager.registerRequest(req);
     });
  }

  // Linking
  _req_manager.subscribeReject(
   [this](const Request & req)
   {
     this->_update_subs.invoke(EngineEvent::RequestRejected, req);
   });

  _buffer_ptr->subscribeRelease(
   [this](const Request & req)
   {
     this->_update_subs.invoke(EngineEvent::BufferReleased, req);
   });
  _buffer_ptr->subscribeRegistered(
   [this](const Request & req)
   {
     this->_update_subs.invoke(EngineEvent::BufferRegistered, req);
   });

  for (auto && device: _device_manager.getDevices())
  {
    device.subscribeRelease(
     [this](const Request & req)
     {
       _update_subs.invoke(EngineEvent::DeviceReleased, req);
     });
    device.subscribeRegistered(
     [this](const Request & req)
     {
       _update_subs.invoke(EngineEvent::DeviceRegistered, req);
     });
  }

  // Linking states
  for (int eventInt = EngineEvent::SourceGenerated; eventInt <= EngineEvent::RequestRejected; ++eventInt)
  {
    EngineEvent event = static_cast< EngineEvent >(eventInt);
    _update_subs.subscribe(event,
     [this, event](const Request & req)
     {
       this->_printState(event, req);
     });
  }
}

void APS::Engine::_printState(EngineEvent event, const Request & req)
{
  static TimeManager::time_unit_t last_time = 0;
  std::string res;

  res += "Time ";
  res += std::to_string(_time_manager_ptr->timeNow());
  res += " (ms) \t";

  res += " Sources: ";
  int i = 0;
  for (auto && source: _sources)
  {
    if (event == EngineEvent::SourceGenerated && i == req.source_id)
    {
      res += GREEN;
      res += "[" + std::to_string(req.source_id) + "]";
    }
    else
    {
      res += BLUE;
      res += "[-]";
    }
    res += DEFAULT;
    res += " ";
    ++i;
  }

  res += " Buffer: ";
  for (auto && buf_item: _buffer_ptr->getBuffer())
  {
    if (buf_item.has_value())
    {
      res += RED;
      res += "[" + std::to_string(buf_item->second.source_id) + "]";
    }
    else
    {
      res += GREEN;
      res += "[-]";
    }
    res += DEFAULT;
    res += " ";
  }

  res += "Devices: ";
  for (auto && device: _device_manager.getDevices())
  {
    if (device.isAvaible())
    {
      res += BLUE;
      res += "[-]";
    }
    else
    {
      res += RED;
      res += "[" + std::to_string(device.getRequest().value().source_id) + "]";
    }
    res += DEFAULT;
    res += " ";
  }
  res += "  " + std::to_string(_time_manager_ptr->timeNow() - last_time) + " (ms) ";
  switch (event)
  {
  case EngineEvent::SourceGenerated:
    res += GREEN;
    res += "Request " + std::to_string(req.source_id) + "." + std::to_string(req.source_req_number) + " generated";
    res += DEFAULT;
    break;
  case EngineEvent::BufferRegistered:
    res += BLUE;
    res += "Buffer registered " + std::to_string(req.source_id) + "." + std::to_string(req.source_req_number) + " request";
    res += DEFAULT;
    break;
  case EngineEvent::BufferReleased:
    res += BLUE;
    res += "Buffer released " + std::to_string(req.source_id) + "." + std::to_string(req.source_req_number) + " request";
    res += DEFAULT;
    break;
  case EngineEvent::DeviceRegistered:
    res += BLUE;
    res += "Device registered " + std::to_string(req.source_id) + "." + std::to_string(req.source_req_number) + " request";
    res += DEFAULT;
    break;
  case EngineEvent::DeviceReleased:
    res += BLUE;
    res += "Device released " + std::to_string(req.source_id) + "." + std::to_string(req.source_req_number) + " request";
    res += DEFAULT;
    break;
  case EngineEvent::RequestRejected:
    res += RED;
    res += "Request " + std::to_string(req.source_id) + "." + std::to_string(req.source_req_number) + " rejected";
    res += DEFAULT;
    break;
  }
  last_time = _time_manager_ptr->timeNow();
  std::cout << res << std::endl;
}

void APS::Engine::run()
{
  while (_createdReq_counter.value() < 100)
  {
    step();
    std::cin.get();
  }

  std::cout << '\n';
  std::cout << "Created: " << _createdReq_counter.value() << '\n';
  std::cout << "Processed: " << _processedReq_counter.value() << '\n';
  std::cout << "Rejected: " << _rejectReq_counter.value() << '\n';
}

void APS::Engine::step()
{
  _time_manager_ptr->stepToEvent();
}

APS::TimeManager::shared APS::Engine::timeManager()
{
  return _time_manager_ptr;
}

APS::Buffer::shared APS::Engine::buffer()
{
  return _buffer_ptr;
}

APS::DeviceManager & APS::Engine::deviceManager()
{
  return _device_manager;
}

APS::SharedCounter::ctype_t APS::Engine::getCreated() const
{
  return _createdReq_counter.value();
}

APS::SharedCounter::ctype_t APS::Engine::getRejected() const
{
  return _rejectReq_counter.value();
}

APS::SharedCounter::ctype_t APS::Engine::getProcessed() const
{
  return _processedReq_counter.value();
}
