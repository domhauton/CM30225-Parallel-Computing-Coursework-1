import multiprocessing
import os

import math

directory = os.path.expanduser('~') + "/jobs"


def get_time_estimate(threads, precision, size, job_type):
    return ((size - 2) * (size - 2) * 3000) / (8000000 * threads)


def get_job_executor(threads, precision, size, job_type):
    return ["./parallel_computation_cw1 {:d} {:d} {:f} {:d}".format(threads, size, precision, job_type)]


def gen_content(threads, job_uid):
    return ["#!/bin/sh",
            "#SBATCH --account=cm30225",
            "#SBATCH --partition=teaching",
            "#SBATCH --job-name={:03d}-{:02d}".format(job_uid, threads),
            "#SBATCH --output=out/{:03d}-{:02d}-%j.out".format(job_uid, threads),
            "#SBATCH --error=out/{:03d}-{:02d}-%j.err".format(job_uid, threads),
            "",
            "#SBATCH --nodes=1",
            "#SBATCH --ntasks-per-node=16".format(threads),
            "",
            "#SBATCH --time=00:15:00",
            "",
            "#SBATCH --mail-type=END",
            "",
            "#SBATCH --mail-user=dh499@bath.ac.uk",
            "# Run the programs"]


def gen_file_name(uid, count):
    return directory + "/cw1-{:03d}-{:02d}.slm".format(count, uid)


def write_to_file(jobs, job_uid):
    filename = gen_file_name(job_uid, len(jobs))
    print("Writing to: " + filename)
    f = open(filename, 'w')
    complete = gen_content(len(jobs), job_uid) + jobs
    f.write("\n".join(complete))
    f.close()


def print_file_name(threads, precision, size, job_type):
    filename = gen_file_name(threads, precision, size, job_type)
    print("Job Target: " + filename)


if not os.path.exists(directory):
    os.makedirs(directory)

job_uid_ctr = 0

jobQueue = []
jobQueueTime = 0
jobQueueMaxTime = 900

currentPrecision = 0.0001
# jobType = 1
# currentThread = 1

for jobType in range(1, 4):
    for currentThread in [1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130]:
    # for currentThread in [2 ** j for j in range(0, int(math.log2(multiprocessing.cpu_count())) + 1)]:
        # for currentThread in [2 * j for j in range(1, int(multiprocessing.cpu_count() / 2) + 1)]:
        for currentSize in [2 ** j for j in range(5, 12)]:
            job_uid_ctr += 1
            jobExecutor = get_job_executor(currentThread, currentPrecision, currentSize, jobType)
            timeEstimate = get_time_estimate(currentThread, currentPrecision, currentSize, jobType)
            print(timeEstimate)
            if timeEstimate > jobQueueMaxTime:
                write_to_file(jobExecutor, job_uid_ctr)
            elif timeEstimate + jobQueueTime > jobQueueMaxTime:
                write_to_file(jobQueue, job_uid_ctr)
                jobQueue = jobExecutor
                jobQueueTime = timeEstimate
            else:
                jobQueue += jobExecutor
                jobQueueTime += jobQueueTime

if len(jobQueue) > 0:
    job_uid_ctr += 1
    write_to_file(jobQueue, job_uid_ctr)
