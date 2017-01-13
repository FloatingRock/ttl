// -*- C++ -*-
#ifndef TTL_LIBRARY_INVERSE_H
#define TTL_LIBRARY_INVERSE_H

#include <ttl/config.h>
#include <ttl/Tensor.h>
#include <ttl/Expressions/traits.h>
#include <ttl/Expressions/force.h>
#include <ttl/Library/determinant.h>
#include <ttl/util/make_index.h>
#include <ttl/util/log2.h>
#include <ttl/util/pow.h>

#ifdef HAVE_MKL_H
#include <mkl.h>
using ipiv_t = MKL_INT;
#else
#include <lapacke.h>
using ipiv_t = lapack_int;
#endif


namespace ttl {
namespace detail {
template <class E,
          int N = expressions::traits<expressions::rinse<E>>::rank::value,
          int D = expressions::traits<expressions::rinse<E>>::dimension::value>
struct square_dimension {
  static constexpr int value = util::pow(D, util::log2<N>::value);
};

template <int N>
struct inverse_impl
{
  static constexpr int lu(double data[N*N], ipiv_t ipiv[N]) {
    return LAPACKE_dgetrf(LAPACK_ROW_MAJOR,N,N,data,N,ipiv);
  }

  static constexpr int inv(double data[N*N], ipiv_t ipiv[N]) {
    return LAPACKE_dgetri(LAPACK_ROW_MAJOR,N,data,N,ipiv);
  }

  template<class E>
  static expressions::tensor_type<E> op(E&& e) {
    ipiv_t ipiv[N];
    auto f = expressions::force(std::forward<E>(e));
    lu(f.data, ipiv);
    inv(f.data, ipiv);
    return f;
  }
};

/// Analytically expand 2x2 inverse.
template <>
struct inverse_impl<2>
{
  template<class E>
  static expressions::tensor_type<E> op(E&& e) {
    auto f = expressions::force(std::forward<E>(e));
    auto t00 = f.eval(util::make_ij(1,1)) ; //a11
    auto t01 = f.eval(util::make_ij(0,1)) ; //a01
    auto t10 = f.eval(util::make_ij(1,0)) ; //a10
    auto t11 = f.eval(util::make_ij(0,0)) ; //a00
    return 1/ttl::det(f) * expressions::tensor_type<E>{t00, -t01,
                                                      -t10,  t11}(ttl::Index<'\0'>(), ttl::Index<'\1'>());
  }
};


  /// Analytically expand 3x3 inverse.
  template <>
  struct inverse_impl<3>
  {
    template<class E>
    static expressions::tensor_type<E> op(E&& e) {
      auto f = expressions::force(std::forward<E>(e));
      auto t00 = f.eval(util::make_ij(2,2))*f.eval(util::make_ij(1,1)) - f.eval(util::make_ij(2,1))*f.eval(util::make_ij(1,2)); //a22a11-a21a12
      auto t01 = f.eval(util::make_ij(2,2))*f.eval(util::make_ij(0,1)) - f.eval(util::make_ij(2,1))*f.eval(util::make_ij(0,2)); //a22a01-a21a02
      auto t02 = f.eval(util::make_ij(1,2))*f.eval(util::make_ij(0,1)) - f.eval(util::make_ij(1,1))*f.eval(util::make_ij(0,2)); //a12a01-a11a02
      auto t10 = f.eval(util::make_ij(2,2))*f.eval(util::make_ij(1,0)) - f.eval(util::make_ij(2,0))*f.eval(util::make_ij(1,2)); //a22a10-a20a12
      auto t11 = f.eval(util::make_ij(2,2))*f.eval(util::make_ij(0,0)) - f.eval(util::make_ij(2,0))*f.eval(util::make_ij(0,2)); //a22a00-a20a02
      auto t12 = f.eval(util::make_ij(1,2))*f.eval(util::make_ij(0,0)) - f.eval(util::make_ij(1,0))*f.eval(util::make_ij(0,2)); //a12a00-a10a02
      auto t20 = f.eval(util::make_ij(2,1))*f.eval(util::make_ij(1,0)) - f.eval(util::make_ij(2,0))*f.eval(util::make_ij(1,1)); //a21a10-a20a11
      auto t21 = f.eval(util::make_ij(2,1))*f.eval(util::make_ij(0,0)) - f.eval(util::make_ij(2,0))*f.eval(util::make_ij(0,1)); //a21a00-a20a01
      auto t22 = f.eval(util::make_ij(1,1))*f.eval(util::make_ij(0,0)) - f.eval(util::make_ij(1,0))*f.eval(util::make_ij(0,1)); //a11a00-a10a01
      return 1/ttl::det(f) * expressions::tensor_type<E>{t00, -t01,  t02,
                                                        -t10,  t11, -t12,
                                                         t20, -t21,  t22}(ttl::Index<'\0'>(), ttl::Index<'\1'>());
    }
  };



} // namespace detail

template <class E>
constexpr expressions::tensor_type<E> inverse(E&& e) {
  return detail::inverse_impl<detail::square_dimension<E>::value>::op(std::forward<E>(e));
}
} // namespace ttl

#endif // #ifndef TTL_LIBRARY_INVERSE_H
