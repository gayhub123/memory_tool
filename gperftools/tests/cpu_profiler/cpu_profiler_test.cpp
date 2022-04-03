#include <iostream>

#include "profiler.h"
#include "simple_mutex.h"

#define SAFE_PTHREAD(fncall)    \
  do {                          \
    if ((fncall) != 0) abort(); \
  } while (0)

static volatile int result = 0;
Mutex mutex(Mutex::LINKER_INITIALIZED);

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
  MutexLock ml(&mutex);
  for (int m = 0; m < 1000000; ++m) {  // run millions of times
    result ^= i;
    snprintf(b, sizeof(b), "other: %d", result);  // get some libc action
  }
}

// This helper function has the signature that pthread_create wants.
static void* RunFunctionInThread(void* ptr_to_ptr_to_fn) {
  (**static_cast<void (**)()>(ptr_to_ptr_to_fn))();  // runs fn
  return NULL;
}

void RunManyThreads(void (*fn)(), int count) {
  pthread_t* thr = new pthread_t[count];
  for (int i = 0; i < count; i++) {
    SAFE_PTHREAD(pthread_create(&thr[i], NULL, RunFunctionInThread, &fn));
  }
  for (int i = 0; i < count; i++) {
    SAFE_PTHREAD(pthread_join(thr[i], NULL));
  }
  delete[] thr;
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
