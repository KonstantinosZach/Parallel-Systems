#!/usr/bin/env python

import subprocess 
import matplotlib.pyplot as plt
import os

def run(output, prog, threads, m, n, type, chunk):
    stdout = open(output, 'w+')
    for i in range(6):
        subprocess.call([prog, threads,  m, n, type, chunk], stdout=stdout)

    time = 0.0

    with open(output) as topo_file:
        counter = 0
        for line in topo_file:
            metric = line.replace('\n','')
            time += float(metric)
            counter += 1
        time /= counter

    stdout.close()
    return time

schedule_type = ['static', 'dynamic', 'guided']
chunks = ['0','1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '100', '500', '1000']

#used to store all the metrics of all the runs (uncomment to get them)
#store = open("store.txt", 'w+')

fig, axs = plt.subplots(len(schedule_type))
fig.set_figheight(15)
fig.set_figwidth(15)
count = 0
list = []

for type in schedule_type:
    # store.write("Type " + type + "\n")
    # store.write("\tChunck\t\tTime\n")
    for chunk in chunks:
        thread_time = run('temp_output.txt','./prog', threads='6', m='8000', n='8000', type=type, chunk=chunk)
        axs[count].bar(chunk, thread_time)
        axs[count].set(xlabel='chunk\n', ylabel='time', title=type)
        list.append((thread_time, type, chunk))
    #     store.write("\t" + chunk + "\t\t\t" + str(thread_time) + "\n")
    # store.write("\n")
    
    count += 1

list.sort()
top5 = list[0:5]
print("Top 5 from List")
print(top5)

fig.suptitle('Triagonial Matrix (8000 x 8000) Multiplication')
plt.show()

os.remove("temp_output.txt")

#we close the file that we created to store all the metrics
# store.close()