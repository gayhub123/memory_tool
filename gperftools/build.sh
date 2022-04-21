#!/bin/sh

ROOT_DIR=$(pwd)
GPERF_SRC_DIR="src/gperftools"
UNWIND_SRC_DIR="src/libunwind"
TEST_BUILD_DIR="build"


build_gperf() {
    echo "building gperftools...see ${GPERF_SRC_DIR}/build.log for more details"
    cd ${ROOT_DIR}
    cd ${GPERF_SRC_DIR}

    sh autogen.sh > build.log
    ./configure >> build.log
    make -j4 >> build.log

    return 0
}

build_unwind() {
    echo "building libunwind...see ${UNWIND_SRC_DIR}/build.log for more details"
    cd ${ROOT_DIR}
    cd ${UNWIND_SRC_DIR}

    autoreconf -i > build.log
    ./configure >> build.log
    make -j4 >> build.log

    return 0
}

build_test() {
    cd ${ROOT_DIR}
    mkdir -p ${TEST_BUILD_DIR}
    cd ${TEST_BUILD_DIR}

    cmake ..
    make -j4

    return 0
}

build_gperf
build_unwind
build_test
