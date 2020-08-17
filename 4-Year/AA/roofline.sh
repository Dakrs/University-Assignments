#PBS -N roofline
#PBS -l walltime=1:00:00
#PBS -l nodes=1:ppn=1:r662
#PBS -q mei

module load papi/5.5.0
module load gcc/4.9.0
module load cuda/7.0.28
source /share/apps/intel/parallel_studio_xe_2019/compilers_and_libraries_2019/linux/bin/compilervars.sh intel64
source /share/apps/intel/advisor_2019.4.0.597843/advixe-vars.sh

cd $PBS_O_WORKDIR

advixe-cl -collect roofline -stack -project-dir roofline_project  ./CPU_AA