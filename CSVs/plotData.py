import csv
import matplotlib.pyplot as plt
from os.path import dirname, join

directory = dirname(__file__)

#Reads BFS data
bfsFile = open(join(directory, 'bfsResults.csv'))
bfsReader = csv.reader(bfsFile)
bfsHeader = next(bfsReader)

bfsX = []
bfsY = []
for pair in bfsReader:
    bfsX.append(float(pair[0]))
    bfsY.append(float(pair[1]))

bfsFile.close()

#Reads A* data
aStarFile = open(join(directory, 'aStarResults.csv'))
aStarReader = csv.reader(aStarFile)
aStarHeader = next(aStarReader)

aStarX = []
aStarY = []
for pair in aStarReader:
    aStarX.append(float(pair[0]))
    aStarY.append(float(pair[1]))

aStarFile.close()

#Plots the data for BFS
plt.plot(bfsX, bfsY, color = "red", label = 'bfs', linewidth = 2)

#Plots the data for A*
plt.plot(aStarX, aStarY, color = "blue", label = 'A*', linewidth = 2)

#Decorates plot
plt.xlabel(bfsHeader[0], fontsize='large')
plt.ylabel(bfsHeader[1], fontsize='large')
plt.legend(fontsize='large')

#Saves the plot in a png
plt.savefig(join(directory, "comparison.png"))
plt.show()
