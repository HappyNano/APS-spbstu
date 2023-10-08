#ifndef APs_BUFFER_HPP
#define APs_BUFFER_HPP

#include <vector>
#include "APS/source.hpp"

namespace APS
{
  class Buffer
  {
   public:
    using this_t = Buffer;

    Buffer(size_t size);
    Buffer(const this_t & obj) = delete;
    Buffer(this_t &&) = delete;

    this_t & operator=(const this_t & obj) = delete;
    this_t & operator=(this_t &&) noexcept = delete;

    /**
     * \brief Check if buffer has avaible slots
     */
    bool is_avaible();
    /**
     * \brief Check if buffer empty
     * \returns true - buffer is empty, false otherwise
     */
    bool is_empty();

    /**
     * \brief Function that gives request by priority.
     * \details First priority is source_id field in request.
     * Second priority is the time the request was buffered.
     * \return Request
     * \throws std::length_error if is_empty()
     */
    Request getRequestByPriority() noexcept(false);

   private:
    std::vector< std::pair< TimeManager::time_unit_t, Request > > _req_memory;
  };
}

#endif
