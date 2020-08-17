module load gcc/5.3.0
module load gcc/7.2.0

export OMP_PROC_BIND=true
export OMP_NUM_THREADS=32

make

./sort 6 5 100000000
