// fcache
// MIT License
//
// Copyright(c) 2023 serhii-shramko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <functional>
#include <map>
#include <tuple>

namespace fcache {
template <typename Signature>
class cached_function;

template <typename ReturnType, typename... ArgTypes>
class cached_function<ReturnType(ArgTypes...)> {
public:
  using function = std::function<ReturnType(ArgTypes...)>;
  using arg_tuple =
      std::tuple<typename std::remove_reference<ArgTypes>::type...>;
  using cache_map = std::map<arg_tuple, ReturnType>;

  ~cached_function() = default;

  cached_function() noexcept {}

  cached_function(std::nullptr_t) noexcept : m_function(nullptr) {}

  cached_function(const cached_function &other)
      : m_function(other.m_function), m_cache(other.m_cache) {}

  cached_function(cached_function &&other) noexcept
      : m_function(std::move(other.m_function)),
        m_cache(std::move(other.m_cache)) {}

  template <class F>
  cached_function(F &&f) : m_function(std::forward<F>(f)) {}

  cached_function &operator=(const cached_function &other) {
    cached_function(other).swap(*this);
    return *this;
  }

  cached_function &operator=(cached_function &&other) {
    cached_function(std::move(other)).swap(*this);
    return *this;
  }

  cached_function &operator=(std::nullptr_t) noexcept {
    m_function = nullptr;
    m_cache.clear();
    return *this;
  }

  template <class F>
  cached_function &operator=(F &&f) {
    cached_function(std::forward<F>(f)).swap(*this);
    return *this;
  }

  template <class F>
  cached_function &operator=(std::reference_wrapper<F> f) noexcept {
    cached_function(f).swap(*this);
    return *this;
  }

  void swap(cached_function &other) noexcept {
    std::swap(m_function, other.m_function);
    std::swap(m_cache, other.m_cache);
  }

  explicit operator bool() const noexcept { return bool(m_function); }

  ReturnType operator()(ArgTypes... args) {
    arg_tuple key(args...);
    auto it = m_cache.find(key);

    if (it != m_cache.end())
      return it->second;

    return (m_cache[key] = m_function(std::forward<ArgTypes>(args)...));
  }

  const std::type_info &target_type() const noexcept {
    return m_function.target_type();
  }

  template <class T>
  T *target() noexcept {
    return m_function.target<T>();
  }

  template <class T>
  const T *target() const noexcept {
    return m_function.target<T>();
  }

private:
  function m_function;
  cache_map m_cache;
};
} // namespace fcache