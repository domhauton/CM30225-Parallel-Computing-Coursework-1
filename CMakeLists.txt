cmake_minimum_required(VERSION 3.6)
project(parallel_computation_cw1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCE_FILES src/main.c src/matrix/matrix.h src/matrix/matrix.c src/matrix/matrixIterator.c src/matrix/matrixIterator.h src/matrix/matrixSmoother.c src/matrix/matrixSmoother.h src/matrix/matrixFactory.c src/matrix/matrixFactory.h src/matrix/matrixEdgeIterator.c src/matrix/matrixEdgeIterator.h)
add_executable(parallel_computation_cw1 ${SOURCE_FILES} src/matrix/matrix.h src/matrix/matrix.c src/matrix/matrixSmoother.c src/matrix/matrixSmoother.h src/matrix/matrixEdgeIterator.c src/matrix/matrixEdgeIterator.h)