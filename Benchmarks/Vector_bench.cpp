#include <benchmark/benchmark.h>

#include <vector>

#include "Source/Vector.hpp"

static void BM_MyVector(benchmark::State& state) {
  uint64_t count = 0;

  MySTL::Vector<int> v;

  for (auto _ : state) {
    v.push_back(count++);
    (void)v.at(static_cast<int>(count / 2));
  }

  state.counters["ops/sec"] =
      benchmark::Counter(static_cast<double>(count), benchmark::Counter::kIsRate);
}

static void BM_STDVector(benchmark::State& state) {
  uint64_t count = 0;

  std::vector<int> v;

  for (auto _ : state) {
    v.push_back(count++);
    (void)v.at(static_cast<int>(count / 2));
  }

  state.counters["ops/sec"] =
      benchmark::Counter(static_cast<double>(count), benchmark::Counter::kIsRate);
}

BENCHMARK(BM_MyVector)->Unit(benchmark::kSecond)->Repetitions(5);
BENCHMARK(BM_STDVector)->Unit(benchmark::kSecond)->Repetitions(5);
BENCHMARK_MAIN();
