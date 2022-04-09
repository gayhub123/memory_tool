cmake_minimum_required(VERSION 3.10)

aux_source_directory(./ src)
add_executable(heap_profiler_test ${src})
target_link_libraries(heap_profiler_test PRIVATE tcmalloc_and_profiler unwind pthread)