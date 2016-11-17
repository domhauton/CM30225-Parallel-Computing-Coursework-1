#!/bin/bash
cd ~
rm ~/jobs/*
python3 bucs/Documents/parallel/src/slurm_job_gen.py
chmod 644 ~/jobs/*
find ./jobs/ -exec sbatch {} \;