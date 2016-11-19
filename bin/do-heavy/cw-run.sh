#!/bin/bash
cd ~
rm ~/jobs/*
python3 ~/parallel/bin/slurm_job_gen.py
chmod 744 ~/jobs/*
find ./jobs/cw* -exec {} \;