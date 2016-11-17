cmake_minimum_required(VERSION 2.8.12.2)
project(parallel_computation_cw1)


set(CMAKE_C_FLAGS "-std=gnu99 -pthread -Wall ${CMAKE_C_FLAGS}")

set(SOURCE_FILES src/main.c src/matrix/matrix.h src/matrix/matrix.c src/matrix/matrixIterator.c src/matrix/matrixIterator.h src/matrix/matrixSmoother.c src/matrix/matrixSmoother.h src/matrix/matrixFactory.c src/matrix/matrixFactory.h src/matrix/matrixEdgeIterator.c src/matrix/matrixEdgeIterator.h src/worker/smoothTask.c src/worker/smoothTask.h src/matrix/matrixParallel.c src/matrix/matrixParallel.h src/matrix/benchmark.h src/matrix/benchmark.c)

add_executable(parallel_computation_cw1 ${SOURCE_FILES} src/matrix/matrix.h src/matrix/matrix.c src/matrix/matrixSmoother.c src/matrix/matrixSmoother.h src/matrix/matrixEdgeIterator.c src/matrix/matrixEdgeIterator.h src/worker/smoothTask.h src/matrix/matrixParallel.c src/matrix/matrixParallel.h src/matrix/benchmark.h src/matrix/benchmark.c)

target_link_libraries(parallel_computation_cw1 m)