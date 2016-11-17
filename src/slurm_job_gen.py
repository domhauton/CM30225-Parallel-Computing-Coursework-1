import os
import multiprocessing
import math

directory = os.path.expanduser('~')+"/jobs"


def gen_ending(threads, precision, size):
    time_estimate_sec = int(int((size*size*(1/precision))/(threads*0.75)) / 30000000)
    if time_estimate_sec > 15*60:
        time_estimate_sec = 15*60
    if time_estimate_sec < 60:
        time_estimate_sec = 60
    return ["#!/bin/sh",
            "#SBATCH --account=cm30225",
            "#SBATCH --partition=teaching",
            "#SBATCH --job-name={:02d}-{:05d}-{:01d}".format(threads, size, -int(math.log10(precision))),
            "#SBATCH --output=out/{:03d}-{:05d}-{:d}-%j.out".format(threads, size, -int(math.log10(precision))),
            "#SBATCH --error=out/{:03d}-{:05d}-{:d}-%j.err".format(threads, size, -int(math.log10(precision))),
            "",
            "#SBATCH --nodes=1",
            "#SBATCH --ntasks-per-node={:d}".format(threads),
            "",
            "#SBATCH --time=00:{:02d}:00".format(int(time_estimate_sec/60)),
            "",
            "#SBATCH --mail-type=END",
            "",
            "#SBATCH --mail-user=dh499@bath.ac.uk",
            "# Run the program",
            "./parallel_computation_cw1 {:d} {:d} {:f}".format(threads, size, precision)]


def gen_file_name(threads, precision, size):
    return directory + "/cw1-{:03d}-{:03d}-{:d}.slm".format(threads, size, -int(math.log10(precision)))


def write_to_file(threads, precision, size):
    filename = gen_file_name(threads, precision, size)
    print("Writing to: " + filename)
    f = open(filename, 'w')
    complete = gen_ending(threads, precision, size)
    f.write("\n".join(complete))
    f.close()


def print_file_name(threads, precision, size):
    filename = gen_file_name(threads, precision, size)
    print("Job Target: " + filename)


if not os.path.exists(directory):
    os.makedirs(directory)

currentPrecision = 0.0001
jobType = 3
for currentThread in [2**j for j in range(0, int(math.log2(multiprocessing.cpu_count()))+1)]:
    for currentSize in [2**j for j in range(6, 12)]:
        write_to_file(currentThread, currentPrecision, currentSize, jobType)
        #print_file_name(currentThread, currentPrecision, currentSize)








