// -------------------------------------------------------------------*- C++ -*-
// Copyright (c) 2017, Center for Shock Wave-processing of Advanced Reactive Materials (C-SWARM)
// University of Notre Dame
// Indiana University
// University of Washington
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// -----------------------------------------------------------------------------
#ifndef TTL_LIBRARY_TRANSPOSE_H
#define TTL_LIBRARY_TRANSPOSE_H

#include <ttl/config.h>
#include <ttl/Expressions/Expression.h>
#include <ttl/Expressions/traits.h>
#include <ttl/Library/binder.h>

namespace ttl {
namespace lib {
template <class T, class Pack> struct
push_back_impl;

template <template <class...> class Pack, class T, class... U>
struct push_back_impl<T, Pack<U...>> {
  using type = Pack<U..., T>;
};

template <class T, class Pack>
using push_back = typename push_back_impl<T, Pack>::type;

template <class Pack>
struct reverse_impl;

template <template <class...> class Pack>
struct reverse_impl<Pack<>>
{
  using type = Pack<>;
};

template <template <class...> class Pack, class T0, class... T>
struct reverse_impl<Pack<T0, T...>>
{
  using type = push_back<T0, typename reverse_impl<Pack<T...>>::type>;
};

template <class Pack>
using reverse = typename reverse_impl<Pack>::type;
} // namespace lib

template <class E>
constexpr auto transpose(E t) {
  using Outer = expressions::outer_type<E>;
  using Type = lib::reverse<Outer>;
  return expressions::Bind<E,Type>(t);
}

template <int R, int D, class S>
constexpr auto transpose(const Tensor<R,D,S>& t) {
  return transpose(lib::bind(t));
}

}// namespace ttl

#endif // #define TTL_LIBRARY_TRANSPOSE_H

