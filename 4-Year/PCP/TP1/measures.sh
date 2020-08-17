#!/bin/sh
#PBS -N pcp_54_781_V0
#PBS -l walltime=1:00:00
#PBS -l nodes=1:ppn=48:r662
#PBS -q mei


module load gcc/5.3.0
module load papi/5.4.1

N_EXECS=6
S_SIZE=1024
VERSION=1
REPETITIONS=100
MAX_THREADS=96
PER=0.2

NODE=652

cd $PBS_O_WORKDIR

if [ ! -d Tests\_$NODE\_$VERSION ]; then
    mkdir Tests\_$NODE\_$VERSION;
else
    rm -r Tests\_$NODE\_$VERSION;
    mkdir Tests\_$NODE\_$VERSION;
fi

DIR=Tests\_$NODE\_$VERSION

for (( i=0; i< $N_EXECS; i++ ))
    do 
        SIZE=$((S_SIZE * (2**i)))
        for j in {2,4,8,10,12,16,20,24,32,40,48,64}
        do
            echo "Size:$SIZE NºThreads:$j" >> $DIR/$SIZE\_$VERSION.txt
            ./sparse $VERSION $PER $SIZE $j $REPETITIONS >> $DIR/$SIZE\_$VERSION.txt
            echo "" >> $DIR/$SIZE\_$VERSION.txt
        done
    done