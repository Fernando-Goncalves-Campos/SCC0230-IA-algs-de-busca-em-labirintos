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
plt.savefig(join(directory, "comparisonBestPath.png"))
plt.clf()

#Reads DFS data
dfsFile = open(join(directory, 'dfsResults.csv'))
dfsReader = csv.reader(dfsFile)
dfsHeader = next(dfsReader)

dfsX = []
dfsY = []
for pair in dfsReader:
    dfsX.append(float(pair[0]))
    dfsY.append(float(pair[1]))

dfsFile.close()

#Reads Best-First data
bestFirstFile = open(join(directory, 'bestFirstResults.csv'))
bestFirstReader = csv.reader(bestFirstFile)
bestFirstHeader = next(bestFirstReader)

bestFirstX = []
bestFirstY = []
for pair in bestFirstReader:
    bestFirstX.append(float(pair[0]))
    bestFirstY.append(float(pair[1]))

bestFirstFile.close()

#Plots the data for DFS
plt.plot(dfsX, dfsY, color = "red", label = 'dfs', linewidth = 2)

#Plots the data for Best-First
plt.plot(bestFirstX, bestFirstY, color = "blue", label = 'Best-First', linewidth = 2)

#Decorates plot
plt.xlabel(dfsHeader[0], fontsize='large')
plt.ylabel(dfsHeader[1], fontsize='large')
plt.legend(fontsize='large')

#Saves the plot in a png
plt.savefig(join(directory, "comparisonLeastWalking.png"))