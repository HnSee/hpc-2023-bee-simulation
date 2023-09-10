while getopts n:h:s:b: flag
do
    case "${flag}" in
        n) num_threads=${OPTARG};;
        h) hives=${OPTARG};;
        s) seed=${OPTARG};;
        b) benchmark=${OPTARG};;
    esac
done

mpirun -n $num_threads builddir/bee_simulation --hives $hives --seed $seed --benchmark $benchmark -e 1000

