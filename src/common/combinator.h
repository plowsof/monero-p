// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.

//
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace tools {

uint64_t combinations_count(uint32_t k, uint32_t n);

template<typename T>
class Combinator {
public:
  Combinator(const std::vector<T>& v) : origin(v) { }

  std::vector<std::vector<T>> combine(size_t k);

private:
  void doCombine(size_t from, size_t k);

  std::vector<T> origin;
  std::vector<std::vector<T>> combinations;
  std::vector<size_t> current;
};

template<typename T>
std::vector<std::vector<T>> Combinator<T>::combine(size_t k)
{
  if (k > origin.size())
  {
    throw std::runtime_error("k must be smaller than elements number");
  }

  if (k == 0)
  {
    throw std::runtime_error("k must be greater than zero");
  }

  combinations.clear();
  doCombine(0, k);
  return combinations;
}

template<typename T>
void Combinator<T>::doCombine(size_t from, size_t k)
{
  current.push_back(0);

  for (size_t i = from; i <= origin.size() - k; ++i)
  {
    current.back() = i;

    if (k > 1) {
      doCombine(i + 1, k - 1);
    } else {
      std::vector<T> comb;
      for (auto ind: current) {
        comb.push_back(origin[ind]);
      }
      combinations.push_back(comb);
    }
  }

  current.pop_back();
}

} //namespace tools
