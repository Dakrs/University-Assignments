#PBS -N gpu_ex1
#PBS -l walltime=0:15:00
#PBS -l nodes=1:ppn=48:r662:k20
#PBS -q mei

module load papi/5.5.0
module load gcc/4.9.0
module load cuda/7.0.28

export CUDA=yes

cd $PBS_O_WORKDIR

./GPU_AA