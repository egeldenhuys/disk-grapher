import numpy as np
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import subprocess
import sys


csv = np.genfromtxt (sys.argv[1], delimiter=",", dtype=int)

startRow = 0

xAxisCol = 4

# x - time since last write, write time
plt.plot(csv[startRow:,xAxisCol], csv[startRow:,1], "rx-", label="Single Pass")

# y - time since last write, average write time
plt.plot(csv[startRow:,xAxisCol], csv[startRow:,2], "bo-", label="Average Pass time")

legend = plt.legend(loc='upper right', shadow=True, fontsize='medium')


#plt.figtext(0, 5 ,'Average Time = ' + csv[-1,2])

plt.title("Buffered USB. Increase sleep after each write.")
plt.ylabel('Write Time (microseconds)')
plt.xlabel('Time since last write (microseconds)')
plt.show()
