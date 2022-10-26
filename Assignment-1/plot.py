import sys
from matplotlib import pyplot as plt
import timeit
import subprocess
 
dataset = sys.argv[1]
output = sys.argv[2]
output_name = ""

if (output[-4:] == ".png"):
    output_name = output
    
else:
    output_name = str(output)+".png"
    
supports = [90, 50, 25, 10, 5]

apriori_ex_times=[]
fptree_ex_times=[]


for s in supports:
    apriori_ex_times.append(timeit.timeit("subprocess.run(\"./apriori " + str(s) + " " + dataset + " apriori_plot.txt\", shell=True)", setup = "import subprocess", number=1))
    fptree_ex_times.append(timeit.timeit("subprocess.run(\"./fptree " + str(s) + " " + dataset + " fptree_plot.txt\", shell=True)", setup = "import subprocess", number=1))
    
plt.figure()
plt.plot(supports, apriori_ex_times, label='Apriori Algorithm')
plt.plot(supports, fptree_ex_times, label='FP-Tree Algorithm')
plt.title('Plot of Execution time v/s Support thresholds')
plt.xlabel('Support thresholds (in %age)')
plt.ylabel('Execution time (s)')
plt.legend()
plt.savefig(output_name)
