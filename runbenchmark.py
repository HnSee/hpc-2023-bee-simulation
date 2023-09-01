import subprocess

num_hives = 100
test_length = 10
max_num_processes = 32
processes = 1           # nicht ändern

subprocess.run([ "module load", "mpi/openmpi" ],shell=True, capture_output=True )               # Das laden der Configuration

# Strong-Scaling Benchmarks
# -> constant problem size
# -> increase in processors


for hives in range(5,num_hives,5):
    for processes in range(1,max_num_processes):
        res = [0]*10
        for seed in range(10):
            b = subprocess.run(["sh","runbenchmark.sh", "-n", str(processes), "-h", str(hives), "-s", str(seed), "-b", str(test_length)], capture_output=True)
            s = b.stdout.decode('utf-8')
            s = s[s.find("Benchmark time:")+16:]
            s = s[:s.find("[")]
            
            res[seed] = float(s)
        print("Process: " + str(processes) + "  -  Hives: " + str(hives))
        print(res)
        print("")

