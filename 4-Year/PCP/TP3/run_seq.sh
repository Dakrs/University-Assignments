#!/bin/sh
#PBS -N pcp
#PBS -l walltime=2:00:00
#PBS -l nodes=2:ppn=32:r641

module load gcc/5.3.0
module load gnu/openmpi_eth/1.8.4

BALANCE=0
SIZE=16384
SEED=1
NODE=641

TARGET=CRS_HYBRID_SEQ

cd $PBS_O_WORKDIR

if [ $TARGET = 'CRS_HYBRID' ]; then
    RESULTS_DIR=Res\_SEQ
else
    RESULTS_DIR=Res\_SEQ
fi

if [ ! -d $RESULTS_DIR ]; then
    mkdir $RESULTS_DIR;
else
    rm -r $RESULTS_DIR;
    mkdir $RESULTS_DIR;
fi

echo "$SIZE;$NP;Tcomm;Tfree;Tcomp;" >> $RESULTS_DIR/results.csv
mpirun -n 1 --map-by ppr:1:socket:PE=8 -mca btl self,sm,tcp  $TARGET $SIZE $BALANCE $NT >> $RESULTS_DIR/results.csv
echo ";;;;;" >> $RESULTS_DIR/results\_$NP.csv