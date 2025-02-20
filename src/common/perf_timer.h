// Copyright (c) The Monero Project
// 
// This source code is licensed under the BSD-3 license found in the
// LICENSE file in the root directory of this source tree.
// 

#pragma once

#include <string>
#include <stdio.h>
#include <memory>
#include "misc_log_ex.h"

namespace tools
{

class PerformanceTimer;

extern el::Level performance_timer_log_level;

uint64_t get_tick_count();
uint64_t get_ticks_per_ns();
uint64_t ticks_to_ns(uint64_t ticks);

class PerformanceTimer
{
public:
  PerformanceTimer(bool paused = false);
  ~PerformanceTimer();
  void pause();
  void resume();
  void reset();
  uint64_t value() const;
  operator uint64_t() const { return value(); }

protected:
  uint64_t ticks;
  bool started;
  bool paused;
};

class LoggingPerformanceTimer: public PerformanceTimer
{
public:
  LoggingPerformanceTimer(const std::string &s, const std::string &cat, uint64_t unit, el::Level l = el::Level::Info);
  ~LoggingPerformanceTimer();

private:
  std::string name;
  std::string cat;
  uint64_t unit;
  el::Level level;
};

void set_performance_timer_log_level(el::Level level);

#define PERF_TIMER_NAME(name) pt_##name
#define PERF_TIMER_UNIT(name, unit) tools::LoggingPerformanceTimer PERF_TIMER_NAME(name)(#name, "perf." MONERO_DEFAULT_LOG_CATEGORY, unit, tools::performance_timer_log_level)
#define PERF_TIMER_UNIT_L(name, unit, l) tools::LoggingPerformanceTimer PERF_TIMER_NAME(name)t_##name(#name, "perf." MONERO_DEFAULT_LOG_CATEGORY, unit, l)
#define PERF_TIMER(name) PERF_TIMER_UNIT(name, 1000000)
#define PERF_TIMER_L(name, l) PERF_TIMER_UNIT_L(name, 1000000, l)
#define PERF_TIMER_START_UNIT(name, unit) std::unique_ptr<tools::LoggingPerformanceTimer> PERF_TIMER_NAME(name)(new tools::LoggingPerformanceTimer(#name, "perf." MONERO_DEFAULT_LOG_CATEGORY, unit, el::Level::Info))
#define PERF_TIMER_START(name) PERF_TIMER_START_UNIT(name, 1000000)
#define PERF_TIMER_STOP(name) do { PERF_TIMER_NAME(name).reset(NULL); } while(0)
#define PERF_TIMER_PAUSE(name) PERF_TIMER_NAME(name).pause()
#define PERF_TIMER_RESUME(name) PERF_TIMER_NAME(name).resume()

}
