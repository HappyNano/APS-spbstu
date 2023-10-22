#ifndef APS_UTILITY_EVENT_SUBSCRIBERS_HPP
#define APS_UTILITY_EVENT_SUBSCRIBERS_HPP

#include <map>
#include "APS/utility/subscribers.hpp"

namespace APS
{
  /**
   * \brief Class Subscribers for Events (Template)
   * \param Args types of params of functions
   */
  template < typename Event, typename... Args >
  class EventSubscribers
  {
   public:
    using this_t = EventSubscribers;
    using subsribers_t = APS::Subscribers< Args... >;
    using function_t = typename subsribers_t::function_t;

    EventSubscribers() = default;
    EventSubscribers(const this_t & obj) = delete;
    EventSubscribers(this_t &&) noexcept;

    this_t & operator=(const this_t & obj) = delete;
    this_t & operator=(this_t &&) noexcept;

    void swap(this_t &) noexcept;

    void subscribe(const Event &, const function_t &);

    void invoke(const Event &, Args... args);

   private:
    std::map< Event, subsribers_t > _subscribers;
  };
}

template < typename Event, typename... Args >
APS::EventSubscribers< Event, Args... >::EventSubscribers(this_t && obj) noexcept:
  _subscribers{ std::move(obj._subscribers) }
{}

template < typename Event, typename... Args >
typename APS::EventSubscribers< Event, Args... >::this_t & APS::EventSubscribers< Event, Args... >::operator=(this_t && obj) noexcept
{
  if (this != std::addressof(obj))
  {
    this_t tmp(std::move(obj));
    swap(obj);
  }
  return *this;
}

template < typename Event, typename... Args >
void APS::EventSubscribers< Event, Args... >::swap(this_t & obj) noexcept
{
  std::swap(_subscribers, obj._subscribers);
}

template < typename Event, typename... Args >
void APS::EventSubscribers< Event, Args... >::subscribe(const Event & event, const function_t & function)
{
  _subscribers[event].subscribe(function);
}

template < typename Event, typename... Args >
void APS::EventSubscribers< Event, Args... >::invoke(const Event & event, Args... args)
{
  auto iter = _subscribers.find(event);
  if (iter != _subscribers.end())
  {
    iter->second.invoke(args...);
  }
}

#endif
