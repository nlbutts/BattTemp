import numpy as np
import matplotlib.pyplot as plt
import time
import sys

def plotData(title, data):
    temp = data[:,1]
    temp = temp / 10
    temp = temp * 9/5 + 32
    tData = data[:,0]

    i = 0
    formattedTime = []
    for t in tData:
        gmt = time.gmtime(t)
        s = time.strftime("%a, %d %b %Y %H:%M:%S", gmt)
        formattedTime.append(s)
        print('{0} : {1}'.format(s, temp[i]))
        i += 1

    plt.plot(temp)
    graphTitle = "Temperature from " + title + " start=" + formattedTime[0] + " end=" + formattedTime[len(tData)-1]
    plt.title(graphTitle)
    plt.ylabel("Temperature (degrees F)")
    plt.grid()
    plt.savefig(title + ".png")
    plt.show()

if sys.argv != 2:
    print("Syntax: python <this file> <file to parse> <title>")

print("Processing file " + sys.argv[1])
data = np.loadtxt(sys.argv[1], delimiter=',')
plotData(sys.argv[2], data)