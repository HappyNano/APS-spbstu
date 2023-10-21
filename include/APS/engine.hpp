#ifndef APS_ENGINE_HPP
#define APS_ENGINE_HPP

#include "APS/source.hpp"
#include "APS/time_manager.hpp"
#include "APS/request_manager.hpp"
#include "APS/buffer.hpp"
#include "APS/device_manager.hpp"

namespace APS
{
  class Engine
  {
   public:
    Engine(size_t sources_count, size_t buffer_size, size_t devices_count, double alpha, double beta, double lambda);

    void printState(int from_source = -1);
    void run();
    void step();

    SharedCounter::ctype_t getCreated() const;
    SharedCounter::ctype_t getRejected() const;
    SharedCounter::ctype_t getProcessed() const;

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

   public:
    //
    APS::Subscribers< const APS::Request & > _update_subs;
  };
}

#endif
