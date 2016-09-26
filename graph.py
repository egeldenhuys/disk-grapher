import numpy as np
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import subprocess
import sys

# Collect data

# for passes in range(1, 2000):
#
# t = np.array([0, 1, 2, 3])
#
# passes
# time = npArr
#

# No Buffer
csv = np.genfromtxt (sys.argv[1], delimiter=",", dtype=int)

# Buffer
csv2 = np.genfromtxt (sys.argv[2], delimiter=",", dtype=int)

print "Average Time: ", csv[-1,2]

# for row in csv:
#     print row[0], row[1]

plt.plot(csv2[:,0], csv2[:,1], "bo", label="Single Buffer")
plt.plot(csv[:,0], csv[:,1], "ro", label="Single No Buffer")

plt.plot(csv2[:,0], csv2[:,2], "b-", label="Average Buffer")
plt.plot(csv[:,0], csv[:,2], "r-", label="Average No Buffer")

legend = plt.legend(loc='upper right', shadow=True, fontsize='medium')


#plt.figtext(0, 5 ,'Average Time = ' + csv[-1,2])

plt.title("Time to save with and without a buffer")
plt.ylabel('Time (microseconds)')
plt.xlabel('Pass')
plt.show()
