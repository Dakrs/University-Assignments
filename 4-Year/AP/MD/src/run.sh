#!/bin/sh

module load gcc/5.3.0
module load gcc/6.1.0
module load gcc/7.2.0

make

export OMP_NUM_THREADS=32
export OMP_PROC_BIND=true

./md 2
