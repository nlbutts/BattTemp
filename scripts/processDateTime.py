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
    formattedTime = range(len(tData))
    for t in tData:
        gmt = time.gmtime(t)
        formattedTime[i] = time.strftime("%a, %d %b %Y %H:%M:%S", gmt)
        i += 1

    plt.plot(temp)
    graphTitle = "Temperature from " + title + " start=" + formattedTime[0] + " end=" + formattedTime[len(tData)-1]
    plt.title(graphTitle)
    plt.ylabel("Temperature (degrees F)")
    plt.grid()
    plt.savefig(title + ".png")
    plt.show()

print("Processing file " + sys.argv[1])
data = np.loadtxt(sys.argv[1], delimiter='-')
plotData(sys.argv[1], data)