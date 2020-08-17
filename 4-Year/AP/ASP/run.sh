module load gcc/5.3.0
module load gcc/6.1.0
module load gcc/7.2.0

source /share/apps/intel/parallel_studio_xe_2019/compilers_and_libraries_2019/linux/bin/compilervars.sh intel64

export OMP_PROC_BIND=true
export OMP_NUM_THREADS=32

make clean

make

./asp
