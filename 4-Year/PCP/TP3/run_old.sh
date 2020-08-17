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

TARGET=CRS_HYBRID_OLD

cd $PBS_O_WORKDIR

if [ $TARGET = 'CRS_HYBRID' ]; then
    RESULTS_DIR=Res\_Par\_OLD
else
    RESULTS_DIR=Res\_Par\_OLD
fi

if [ ! -d $RESULTS_DIR ]; then
    mkdir $RESULTS_DIR;
else
    rm -r $RESULTS_DIR;
    mkdir $RESULTS_DIR;
fi

for NP in {1,2,4}
do
    for NT in {4,8,16}
    do
        for j in {1}
        do
            echo "$SIZE;$NP;Tcomm;Tfree;Tcomp;" >> $RESULTS_DIR/results\_$NP.csv
            mpirun -n $NP --map-by ppr:1:socket:PE=8 -mca btl self,sm,tcp  $TARGET $SIZE $BALANCE $NT >> $RESULTS_DIR/results\_$NP.csv
            echo ";;;;;" >> $RESULTS_DIR/results\_$NP.csv
        done
    done
done