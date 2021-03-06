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
#include <ttl/ttl.h>
#include <gtest/gtest.h>

const ttl::Index<'i'> i;
const ttl::Index<'j'> j;

TEST(Determinant, Basic_2_2) {
  ttl::Tensor<2,2,int> A = {1,2,3,4};
  int d = ttl::det(A);
  EXPECT_EQ(d, -2);
}

TEST(Determinant, External_2_2) {
  int a[4];
  const ttl::Tensor<2,2,int*> A = {a, {1,2,3,4}};
  int d = ttl::det(A);
  EXPECT_EQ(d, -2);
}

TEST(Determinant, RValue_2_2) {
  int d = ttl::det(ttl::Tensor<2,2,int>{1,2,3,4});
  EXPECT_EQ(d, -2);
}

TEST(Determinant, ExpressionRValue_2_2) {
  ttl::Tensor<2,2,int> A = {1,2,3,4};
  int d = ttl::det(A(i,j));
  EXPECT_EQ(d, -2);
}

TEST(Determinant, Expression_2_2) {
  ttl::Tensor<2,2,int> A = {1,2,3,4};
  auto e = A(i,j);
  int d = ttl::det(e);
  EXPECT_EQ(d, -2);
}

TEST(Determinant, 3_3) {
  ttl::Tensor<2,3,int> A = {1,2,3,
                            4,5,6,
                            7,8,10};
  int d = ttl::det(A);
  EXPECT_EQ(d, -3);
}
