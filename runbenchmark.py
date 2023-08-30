import subprocess

num_hives = 1
test_length = 10
max_num_processes = 4   # muss immer 2^x sein

subprocess.run([ "module load", "mpi/openmpi" ],shell=True, capture_output=True )               # Das laden der Configuration

# Strong-Scaling Benchmarks
# -> constant problem size
# -> increase in processors

processes = 1
while processes <= max_num_processes:
    print(processes)
    b = subprocess.run(["mpirun", "-n", str(processes), "builddir/bee_simulation", "--hives", "3", "--seed", "0", "--benchmark", "5"], capture_output=True )
    s = b.stdout.decode('utf-8')
    num = s[(s.find("Benchmark time:")+15):]
    print("Processes: ",processes," - ","Time: ",float(num[1:-2]))
    processes *= 2

