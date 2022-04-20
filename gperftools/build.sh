#!/bin/sh

ROOT_DIR=$(pwd)
SRC_BUILD_DIR="src/build"
TEST_BUILD_DIR="build"


build_src() {
    mkdir -p ${SRC_BUILD_DIR}
    cd ${SRC_BUILD_DIR}

    cmake ..
    make -j4

    return 0
}

build_test() {
    mkdir -p ${TEST_BUILD_DIR}
    cd ${TEST_BUILD_DIR}

    cmake ..
    make -j4

    return 0
}

cd ${ROOT_DIR}
build_src

cd ${ROOT_DIR}
build_test
