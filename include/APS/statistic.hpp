#ifndef APS_STATISTIC_HPP
#define APS_STATISTIC_HPP

#include "APS/request.hpp"

namespace APS
{
  class SourceStatistic
  {
   public:
    SourceStatistic(int source_id, const std::vector< Request > & requests);
    ~SourceStatistic() = default;

    int id() const;
    size_t countCreated() const;
    double p_rejected() const;
    double average_TimeIn() const;
    double average_TimeBuffered() const;
    double average_TimeService() const;
    double dispersion_TimeBuffered() const;
    double dispersion_TimeService() const;

   private:
    int _source_id;           // ID источника
    size_t _count_createdReq; // Количество заявок
    double _p_rejected;       // Вероятность отказа с данного источника
    double _average_T_in;     // Среднее время пребывания в системе
    // if (!rejected) processed_time - created_time
    double _average_T_buffered; // Среднее время пребывания в буфере
    // if (!rejected) registered_time - buffered_time
    double _average_T_service; // Среднее время обслуживания
    // if (!rejected) processed_time - registered_time
    double _dispersion_T_buffered;
    double _dispersion_T_service;
  };

  class DeviceStatistic
  {
   public:
    DeviceStatistic(int device_id, const std::vector< Request > & requests);
    ~DeviceStatistic() = default;

    int id() const;
    double usingCoef() const;

   private:
    int _device_id;
    double _using_coef;
  };

  class Statistic
  {
   public:
    /**
     * \brief Constructor
     * \param requests is vector of out model requests
     */
    Statistic(const std::vector< Request > & requests);

    size_t count_createdReq() const;
    size_t count_processedReq() const;
    size_t count_rejectedReq() const;
    const std::map< int, SourceStatistic > & sources() const;
    const std::map< int, DeviceStatistic > & devices() const;

   private:
    size_t _count_createdReq;
    size_t _count_processedReq;
    size_t _count_rejectedReq;
    std::map< int, SourceStatistic > _sources;
    std::map< int, DeviceStatistic > _devices;
  };
}

#endif
