cmake_minimum_required(VERSION 3.10)

aux_source_directory(./ src)
add_executable(cpu_profiler_test ${src})
target_link_libraries(cpu_profiler_test PRIVATE profiler unwind pthread)