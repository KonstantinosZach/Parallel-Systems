#!/usr/bin/env python

import subprocess 
import matplotlib.pyplot as plt
import os

def run(output, prog, arg1, arg2):
    stdout = open(output, 'w+')
    for i in range(4):
        subprocess.call([prog, arg1,  arg2], stdout=stdout)

    time = 0.0

    with open(output) as topo_file:
        counter = 0
        for line in topo_file:
            metrics_list = line.replace('\n','').split(' ')
            time += float(metrics_list[1])
            counter += 1
        time /= counter

    stdout.close()
    return time

args1_list = ['10^4', '10^5', '10^6', '10^7', '10^8'] #sum of darts
args2_list = ['1', '2', '3', '4', '5', '6', '7', '10', '12'] #number of threads

time = [10**4,10**5,10**6,10**7,10**8] #will be used to plot sum of darts (identical with args1_list)

#used to store all the metrics of all the runs (uncomment to get them)
#store = open("store.txt", 'w+')

for arg2 in args2_list:
    results = []    #will be used to plot time
    
    #store.write("Threads " + arg2 + "\n")
    #store.write("\tSum of darts\t\tTime\n")
    
    for arg1 in args1_list:
        thread_time = run('temp_output.txt','./prog', arg1=arg1, arg2=arg2)
        results.append(thread_time)
        
        #store.write("\t" + arg1 + "\t\t\t\t" + str(thread_time) + "\n")
    #store.write("\n")
    
    #for each thread we plot the time needed to execute the program
    label = "thread " + arg2
    plt.plot(time, results, label=label)

#finalize the plots
plt.xlabel('sum of darts')
plt.ylabel('time')
plt.title('Monte Carlo OpenMP')
plt.legend()
plt.show()

#we delete the file that we created to store the temp times
os.remove("temp_output.txt")

#we close the file that we created to store all the metrics
#store.close()