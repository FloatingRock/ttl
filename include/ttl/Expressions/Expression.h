// -*- C++ -*-
#ifndef TTL_EXPRESSIONS_EXPRESSION_H
#define TTL_EXPRESSIONS_EXPRESSION_H

#include <ttl/Expressions/force.h>
#include <ttl/Expressions/traits.h>
#include <type_traits>

namespace ttl {
namespace expressions {

/// Forward declare the IndexMap template, since it is needed in the Expression
// template <class T, class Outer, class Inner> class IndexMap;
template <class T, class Index> class Bind;

/// The base expression class template.
///
/// Every expression will subclass this template, with it's specific subclass as
/// the E type parameter. This curiously-recurring-template-pattern provides
/// static polymorphic behavior, allowing us to build expression trees.
///
/// @tparam           E The type of the base expression.
template <class E>
class Expression {
 public:
  template <class I>
  constexpr auto eval(I index) const {
    return static_cast<const E*>(this)->eval(index);
  }

  template <class... I>
  constexpr const auto to(I... index) const {
    return Bind<const Expression<E>, std::tuple<I...>>(*this, std::make_tuple(index...));
    // return Bind<E, std::tuple<I...>,
    //                 outer_type<E>>(static_cast<const E&>(*this));
  }

  constexpr operator scalar_type<E>() const {
    static_assert(std::tuple_size<outer_type<E>>::value == 0,
                  "Cannot use Tensor as scalar");
    return static_cast<const E*>(this)->eval(std::tuple<>{});
  }
};

template <class E>
struct traits<Expression<E>> : public traits<E> {
};

namespace detail {
template <class E>
struct is_expression_impl {
  using type = std::is_base_of<Expression<E>, E>;
};
} // namespace detail

template <class E>
using is_expression = typename detail::is_expression_impl<E>::type;

} // namespace expressions
} // namespace ttl

#endif // TTL_EXPRESSIONS_EXPRESSION_H
