import numpy as np
import matplotlib.pyplot as plt
import subprocess
import sys
import time

def generateGraph(csvPath1, csvPath2, startRow, xAxisCol, title, xlabel, imagePath, show=False):

    csv1 = np.genfromtxt (csvPath1, delimiter=",", dtype=long)

    # y axis cols: 1 (pass time), 2 (average pass time)
    plt.plot(csv1[startRow:,xAxisCol], csv1[startRow:,1], "rx", label="Single Pass. No Buffer")
    plt.plot(csv1[startRow:,xAxisCol], csv1[startRow:,2], "r-", label="Average Pass time. No Buffer")

    if csvPath2 != "":
        csv2 = np.genfromtxt (csvPath2, delimiter=",", dtype=long)

        plt.plot(csv2[startRow:,xAxisCol], csv2[startRow:,1], "bx", label="Single Pass. Buffer")
        plt.plot(csv2[startRow:,xAxisCol], csv2[startRow:,2], "b-", label="Average Pass time. Buffer")

    legend = plt.legend(loc='upper right', shadow=True, fontsize='small')

    plt.title(title)
    plt.ylabel('Write Time (microseconds)')
    #plt.xlabel(xlabel)
    if xAxisCol == 0:
        plt.xlabel('Pass Number')

    if xAxisCol == 4:
        plt.xlabel('Sleep Time (microseconds)')

    plt.savefig(imagePath)

    if show:
        plt.show()

    plt.clf()   # Clear figure

##################
# Config
##################
diskWriterPath = "./diskWriter"
usbfilePath = "/media/john-bool/Evert/test.txt"
show = True

# ./resultGenerator.out -w /media/john-bool/Evert/test.txt -p 200 -s 0 -t 10000 -b 0 -l 0.csv
#################################
# USB, find sleep threshold QUICK
#################################

# p = subprocess.Popen([diskWriterPath, '-w', usbfilePath, '-s', '0', '-p', '10', '-t', '1000000', '-b', '0', '-l', 'usb_t_q_0.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# generateGraph('usb_t_q_0.csv', "", 1, 4, "Write time to USB, increasing sleep intervals to find threshold", "", "usb_t_q.png", show)

#############################
# USB, find sleep threshold
##############################
# Threshold = 800000 microseconds


# p = subprocess.Popen([diskWriterPath, '-w', usbfilePath, '-s', '0', '-p', '200', '-t', '10000', '-b', '0', '-l', 'usb_t_0.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# time.sleep(5)
# #200
# p = subprocess.Popen([diskWriterPath, '-w', usbfilePath, '-s', '0', '-p', '200', '-t', '10000', '-b', '1', '-l', 'usb_t_1.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# generateGraph('usb_t_0.csv', "usb_t_1.csv", 1, 4, "Write time to USB, increasing sleep interval to find threshold", "", "usb_t.png")

####################################################
# USB, compare buffered performance, using threshold
####################################################
# threshold = 1000000

# p = subprocess.Popen([diskWriterPath, '-w', usbfilePath, '-s', '1000000', '-p', '200', '-t', '0', '-b', '0', '-l', 'usb_b_t_0.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# time.sleep(5)
#
# p = subprocess.Popen([diskWriterPath, '-w', usbfilePath, '-s', '1000000', '-p', '200', '-t', '0', '-b', '1', '-l', 'usb_b_t_1.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# generateGraph('usb_b_t_0.csv', "usb_b_t_1.csv", 1, 0, "Write time to USB, sleep = 1 s", "", "usb_b_t.png", show)

####################################################
# USB, compare buffered performance, no sleep
####################################################
# threshold = 1000000

# p = subprocess.Popen([diskWriterPath, '-w', usbfilePath, '-s', '0', '-p', '200', '-t', '0', '-b', '0', '-l', 'usb_b_0.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# time.sleep(5)
#
# p = subprocess.Popen([diskWriterPath, '-w', usbfilePath, '-s', '0', '-p', '200', '-t', '0', '-b', '1', '-l', 'usb_b_1.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# generateGraph('usb_b_0.csv', "usb_b_1.csv", 1, 0, "Write time to USB, no sleep", "", "usb_b.png", show)


############################
# HDD, find sleep threshold
############################
# Threshold = 500 microseconds

# p = subprocess.Popen([diskWriterPath, '-w', 'test.txt', '-d', '2', '-u', '0', '-s', '0', '-p', '2000', '-t', '10', '-b', '0', '-l', 'hdd_t_0.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# time.sleep(5)
#
# p = subprocess.Popen([diskWriterPath, '-w', "test.txt", '-d', '2', '-u', '0', '-s', '0', '-p', '2000', '-t', '10', '-b', '1', '-l', 'hdd_t_1.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# generateGraph('hdd_t_0.csv', "hdd_t_1.csv", 1, 4, "Write time to HDD, increasing sleep to find threshold", "", "hdd_t.png", show)

##########################################################
# HDD, compare buffer and no buffer, using sleep threshold
##########################################################
# Threshold = 500 microseconds

# p = subprocess.Popen([diskWriterPath, '-w', 'test.txt', '-s', '500', '-d', '1.5', '-u', '0', '-p', '200', '-t', '0', '-b', '0', '-l', 'hdd_b_t_0.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# time.sleep(5)
#
# p = subprocess.Popen([diskWriterPath, '-w', 'test.txt', '-s', '500', '-d', '1.5', '-u', '0', '-p', '200', '-t', '0', '-b', '1', '-l', 'hdd_b_t_1.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# generateGraph('hdd_b_t_0.csv', "hdd_b_t_1.csv", 1, 0, "Write time to HDD, constant sleep at threshold", "", "hdd_b_t.png", show)

##########################################################
# HDD, compare buffer and no buffer, using no sleep
##########################################################
#
# p = subprocess.Popen([diskWriterPath, '-w', 'test.txt', '-s', '0', '-d', '5', '-u', '0', '-p', '2000', '-t', '0', '-b', '0', '-l', 'hdd_b_0.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# time.sleep(5)
#
# p = subprocess.Popen([diskWriterPath, '-w', "test.txt", '-s', '0', '-d', '5', '-u', '0', '-p', '2000', '-t', '0', '-b', '1', '-l', 'hdd_b_1.csv'],
#     stdout=subprocess.PIPE, stderr=subprocess.PIPE)
# out, err = p.communicate()
# print err
# print out
#
# generateGraph('hdd_b_0.csv', "hdd_b_1.csv", 1, 0, "Write time to HDD, no sleep", "", "hdd_b.png", show)
