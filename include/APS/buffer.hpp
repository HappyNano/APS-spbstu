#ifndef APs_BUFFER_HPP
#define APs_BUFFER_HPP

#include <vector>
#include <optional>
#include "APS/source.hpp"

namespace APS
{
  class Buffer
  {
   public:
    using this_t = Buffer;
    using shared = std::shared_ptr< Buffer >;

    Buffer(size_t size);
    Buffer(const this_t & obj) = delete;
    Buffer(this_t &&) = delete;

    this_t & operator=(const this_t & obj) = delete;
    this_t & operator=(this_t &&) noexcept = delete;

    /**
     * \brief Check if buffer has avaible slots
     * \returns true - buffer is avaible (there are available places in the buffer),
     *          false otherwise
     */
    bool is_avaible() const;
    /**
     * \brief Check if buffer empty
     * \returns true - buffer is empty,
     *          false otherwise
     */
    bool is_empty() const;

    /**
     * \brief Function that gives request by priority.
     * \details First priority is source_id field in request.
     *          Second priority is the time the request was buffered.
     * \return Request
     * \throws std::length_error if is_empty()
     */
    Request getRequestByPriorityThrow() noexcept(false);

    /**
     * \brief Function that gives request by priority.
     * \details First priority is source_id field in request.
     *          Second priority is the time the request was buffered.
     * \return Request
     */
    std::optional< Request > getRequestByPriority() noexcept(true);

    /**
     * \brief Functions that registering incoming request
     * \param req is incoming request
     */
    void registerRequest(const Request & req);

    static shared makeShared(size_t size);

   private:
    std::vector< std::pair< TimeManager::time_unit_t, Request > > _req_memory;
    APS::Subscribers<> _subs;
  };
}

#endif
