#ifndef APS_UTILITY_SUBSCRIBERS_HPP
#define APS_UTILITY_SUBSCRIBERS_HPP

#include <vector>
#include <functional>
#include <type_traits>

namespace APS
{
  /**
   * \brief Class Subscribers (Template)
   * \param F - function type
   */
  template < typename... Args >
  class Subscribers
  {
   public:
    using this_t = Subscribers;
    using function_t = std::function< void(Args...) >;

    static_assert(std::is_invocable_v< function_t, Args... >, "Incorrect argument types");

    Subscribers() = default;
    Subscribers(const this_t & obj) = delete;
    Subscribers(this_t &&) = delete;

    this_t & operator=(const this_t & obj) = delete;
    this_t & operator=(this_t &&) noexcept = delete;

    void subscribe(const function_t &);

    void invoke(Args... args);

   private:
    std::vector< function_t > _subscribers;
  };
}

template < typename... Args >
void APS::Subscribers< Args... >::subscribe(const function_t & function)
{
  _subscribers.push_back(function);
}

template < typename... Args >
void APS::Subscribers< Args... >::invoke(Args... args)
{
  for (auto && subscriber: _subscribers)
  {
    subscriber(args...);
  }
}

#endif
