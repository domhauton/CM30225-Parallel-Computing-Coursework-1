//
// Created by dominic on 16/11/16.
//

#ifndef PARALLEL_COMPUTATION_CW1_BENCHMARK_H
#define PARALLEL_COMPUTATION_CW1_BENCHMARK_H

double Benchmark_smoothUntilLimit(int size, double precision, int threads);

double Benchmark_smoothUntilLimitPooled(int size, double precision, unsigned int threads);

double Benchmark_smoothUntilLimitPooledCut(int size, double precision, unsigned int threads);

double Benchmark_serial(int size, double precision);

#endif //PARALLEL_COMPUTATION_CW1_BENCHMARK_H
