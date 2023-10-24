#ifndef APS_ENGINE_HPP
#define APS_ENGINE_HPP

#include "APS/source.hpp"
#include "APS/time_manager.hpp"
#include "APS/request_manager.hpp"
#include "APS/buffer.hpp"
#include "APS/device_manager.hpp"
#include "APS/utility/event_subscribers.hpp"
#include "APS/statistic.hpp"

namespace APS
{
  enum EngineEvent
  {
    // Source
    SourceGenerated,
    // Buffer
    BufferRegistered,
    BufferReleased,
    // Device
    DeviceRegistered,
    DeviceReleased,

    RequestRejected
  };

  class Engine
  {
   public:
    Engine(size_t sources_count,
     size_t buffer_size,
     size_t devices_count,
     double alpha,
     double beta,
     double lambda,
     std::optional< size_t > req_count = {});

    void run();
    void step();

    APS::TimeManager::shared timeManager();
    APS::Buffer::shared buffer();
    APS::DeviceManager & deviceManager();

    SharedCounter::ctype_t getCreated() const;
    SharedCounter::ctype_t getRejected() const;
    SharedCounter::ctype_t getProcessed() const;

    APS::Statistic collectStat() const;
    bool stoped() const;

   private:
    // Counters
    APS::SharedCounter _rejectReq_counter;
    APS::SharedCounter _createdReq_counter;
    APS::SharedCounter _processedReq_counter;

    // Logic classes
    APS::TimeManager::shared _time_manager_ptr;
    std::vector< Source::shared > _sources;
    APS::Buffer::shared _buffer_ptr;
    APS::RequestManager _req_manager;
    APS::DeviceManager _device_manager;

    // Statistic's vector
    std::shared_ptr< std::vector< Request > > _outModel_requests;

    void _printState(EngineEvent, const Request &);

   public:
    //
    APS::EventSubscribers< APS::EngineEvent, const APS::Request & > _update_subs;
  };
}

#endif
