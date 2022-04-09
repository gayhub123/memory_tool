#include <iostream>
#include <string>

#include "basictypes.h"
#include "heap-profiler.h"

static const int kMaxCount = 100000;
int* g_array[kMaxCount] = {nullptr};

static ATTRIBUTE_NOINLINE void Allocate(int start, int end, int size) {
  for (int i = start; i < end; ++i) {
    if (i < kMaxCount) g_array[i] = new int[size];
  }
}

static void Deallocate(int start, int end) {
  for (int i = start; i < end; ++i) {
    delete[] g_array[i];
    g_array[i] = nullptr;
  }
}

static void Clear() {
  for (int i = 0; i < kMaxCount; ++i) {
    if (g_array[i] != nullptr) {
      delete[] g_array[i];
      g_array[i] = nullptr;
    }
  }
}

static void test1() {
  Allocate(0, 1000, 1024);
  Deallocate(0, 10);
  Deallocate(10, 20);
  Deallocate(90, 100);
  Deallocate(20, 90);
}

static void test2() {
  Allocate(0, 40, 100);
  Deallocate(0, 40);

  Allocate(0, 40, 100);
  Allocate(0, 40, 100);
  Allocate(40, 400, 1000);
  Allocate(400, 1000, 10000);
  Deallocate(0, 1000);
}

static void test3() { Allocate(0, kMaxCount, 100); }

int main(int argc, char** argv) {
  std::string filename{"heap_test.profile"};
  HeapProfilerStart(filename.c_str());

  test1();
  test2();
  test3();

  HeapProfilerDump("exit");
  HeapProfilerStop();

  Clear();

  return 0;
}
