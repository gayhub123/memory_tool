#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "gperftools/profiler.h"

static volatile int result = 0;
std::mutex mutex;

static void test_main_thread() {
  int i = 0;
  char b[128];
  for (int m = 0; m < 1000000; ++m) {  // run millions of times
    result ^= i;
    snprintf(b, sizeof(b), "same: %d", result);  // get some libc action
  }
}

static void test_other_thread() {
  ProfilerRegisterThread();

  int i = 0;
  char b[128];
  std::lock_guard<std::mutex> lock(mutex);
  for (int m = 0; m < 1000000; ++m) {  // run millions of times
    result ^= i;
    snprintf(b, sizeof(b), "other: %d", result);  // get some libc action
  }
}

void RunManyThreads(void (*fn)(), int count) {
  std::vector<std::thread> threads;
  for (auto i = 0; i < count; i++) {
    std::thread t(fn);
    threads.push_back(std::move(t));
  }
  for (auto i = 0; i < count; i++) {
    threads[i].join();
  }
}

int main(int argc, char* argv[]) {
  const char* filename = "cpu_test.profile";
  ProfilerStart(filename);

  ProfilerFlush();  // just because we can

  test_main_thread();
  RunManyThreads(test_other_thread, 4);
  test_main_thread();

  ProfilerStop();

  return 0;
}
