#pragma once

#include <vector>
#include <array>
#include <stack>
#include <random>

#include "grid.hpp"

class Cave : public Grid{

public:
    Cave(const size_t size, const int startY_, const int startX_,
         const int endY_, const int endX_, const int fillPercent = 50, const int nIterations = 5)
         : Grid(size, startY_, startX_, endY_, endX_){
        //Creates a maze the that uses sizeXsize spaces
        if(size == 0)
            return;

        fillCave(fillPercent);

        cellAutomata(nIterations);

        expandkeyPoints(2);

        paths[startY][startX] |= paint::START;
        paths[endY][endX] |= paint::GOAL;

        toImage("../images/ppmFiles/unsolvedCaveBeforeConnecting");

        connectRegions();
        
        movesFromPaths();

        paths[startY][startX] |= paint::START;
        paths[endY][endX] |= paint::GOAL;
    }

private:
    void expandkeyPoints(const int radius = 0){
        paintCircle(startY, startX, radius, paint::PATH);
        paintCircle(endY, endX, radius, paint::PATH);
    }

    void fillCave(const int fillPercent){
        std::random_device rd;

        for(int i = 0; i < paths.size(); i++){
            for(int j = 0; j < paths[i].size(); j++){
                if(rd() % 101 > fillPercent){
                    paths[i][j] = Grid::paint::PATH;
                }
            }
        }
    }

    void cellAutomata(const int nIterations){
        for(int n = 0; n < nIterations; n++){
            for(int i = 0; i < paths.size(); i++){
                for(int j = 0; j < paths[i].size(); j++){
                    int neighborCount = countNeighbors(i, j);
                    if(neighborCount > 4){
                        paths[i][j] = Grid::paint::PATH;
                    }
                    else if(neighborCount != 4){
                        paths[i][j] = 0;
                    }
                }
            }
        }
    }

    int countNeighbors(const int i, const int j) const {
        int count = 0;
        
        int baseI = i - (i != 0);
        int finalI = i + (i != paths.size()-1);

        int baseJ = j - (j != 0);
        int finalJ = j + (j != paths[i].size());

        for(int lin = baseI; lin <= finalI; lin++){
            for(int col = baseJ; col <= finalJ; col++){
                count += (paths[lin][col] != 0);
            }
        }

        return count - (paths[i][j] != 0);
    }
    
    struct Region{
        int id;
        std::vector<std::array<int, 2>> edgeNodes;
        bool connectedToStart;
    };
    
    std::vector<Region> getRegions() const {
        std::vector<std::vector<char>> visited(paths.size(), std::vector<char>(paths.size(), 0));
        std::vector<Region> regions;

        int regionID = 0;
        for(int i = 0; i < paths.size(); i++){
            for(int j = 0; j < paths[i].size(); j++){
                if(paths[i][j] == 0 || visited[i][j] != 0)
                    continue;

                visited[i][j] = 1;

                std::stack<std::array<int, 2>> nodes;
                nodes.push({i, j});

                Region newRegion;
                newRegion.connectedToStart = false;
                newRegion.id = regionID++;

                while(!nodes.empty()){
                    std::array<int, 2> curNode = nodes.top();
                    nodes.pop();

                    if(curNode[0] == startY && curNode[1] == startX){
                        newRegion.connectedToStart = true;
                    }

                    int count = 0;
                    if(curNode[0] != 0){
                        if(paths[curNode[0] - 1][curNode[1]] && visited[curNode[0] - 1][curNode[1]] == 0){
                            nodes.push({curNode[0] - 1, curNode[1]});
                            visited[curNode[0] - 1][curNode[1]] = 1;
                        }
                        else if(paths[curNode[0] - 1][curNode[1]] == 0){
                            count++;
                        }
                    }
                    if(curNode[1] != paths[curNode[0]].size() - 1){
                        if(paths[curNode[0]][curNode[1] + 1] && visited[curNode[0]][curNode[1] + 1] == 0){
                            nodes.push({curNode[0], curNode[1] + 1});
                            visited[curNode[0]][curNode[1] + 1] = 1;
                        }
                        else if(paths[curNode[0]][curNode[1] + 1] == 0){
                            count++;
                        }
                    }
                    if(curNode[0] != paths.size() - 1){
                        if(paths[curNode[0] + 1][curNode[1]] && visited[curNode[0] + 1][curNode[1]] == 0){
                            nodes.push({curNode[0] + 1, curNode[1]});
                            visited[curNode[0] + 1][curNode[1]] = 1;
                        }
                        else if(paths[curNode[0] + 1][curNode[1]] == 0){
                            count++;
                        }
                    }
                    if(curNode[1] != 0){
                        if(paths[curNode[0]][curNode[1] - 1] && visited[curNode[0]][curNode[1] - 1] == 0){
                            nodes.push({curNode[0], curNode[1] - 1});
                            visited[curNode[0]][curNode[1] - 1] = 1;
                        }
                        else if(paths[curNode[0]][curNode[1] - 1] == 0){
                            count++;
                        }
                    }

                    if(count){
                        newRegion.edgeNodes.push_back(curNode);
                    }
                }

                regions.push_back(newRegion);
            }
        }

        return regions;
    }

    void joinRegions(const int region1, const int region2, const std::array<int, 2>& node1, const std::array<int, 2>& node2,
                     std::vector<std::vector<char>>& connectedRegions, std::vector<Region>& regions){
        for(int i = 0; i < connectedRegions.size(); i++){
            connectedRegions[region1][i] ^= connectedRegions[region2][i];
            connectedRegions[region2][i] = connectedRegions[region1][i];
        }
        regions[region1].connectedToStart ^= regions[region2].connectedToStart;
        regions[region2].connectedToStart = regions[region1].connectedToStart;

        std::vector<std::array<int, 2>> line = getLine(node1[0], node1[1], node2[0], node2[1]);
        for(const std::array<int, 2>& node : line){
            paintCircle(node[0], node[1], 4, paint::PATH);
        }
    }
    
    void connectClosestRegions(std::vector<Region>& regions, std::vector<std::vector<char>>& connectedRegions){
        std::vector<Region> regionsA;
        std::vector<Region> regionsB;

        for(const Region& region : regions){
            if(region.connectedToStart)
                regionsB.push_back(region);
            else
                regionsA.push_back(region);
        }
        
        int bestDist = 0x7fffffff;
        int bestRegionA = 0;
        int bestRegionB = 0;
        std::array<int, 2> bestNodeA;
        std::array<int, 2> bestNodeB;
        bool foundConnection = false;
        
        for(Region& regionA : regionsA){
            for(Region& regionB : regionsB){
                if(connectedRegions[regionA.id][regionB.id]){
                    continue;
                }

                for(const std::array<int, 2>& nodeA : regionA.edgeNodes){
                    for(const std::array<int, 2>& nodeB : regionB.edgeNodes){
                        const int dist = (nodeA[0] - nodeB[0]) * (nodeA[0] - nodeB[0]) + (nodeA[1] - nodeB[1]) * (nodeA[1] - nodeB[1]);
                        if(dist < bestDist){
                            bestDist = dist;
                            bestRegionA = regionA.id;
                            bestRegionB = regionB.id;
                            bestNodeA = nodeA;
                            bestNodeB = nodeB;
                            foundConnection = true;
                        }
                    }
                }
            }
        }

        if(foundConnection){
            joinRegions(bestRegionA, bestRegionB, bestNodeA, bestNodeB, connectedRegions, regions);
            connectClosestRegions(regions, connectedRegions);
        }
    }

    void connectRegions(){
        std::vector<Region> regions = getRegions();

        std::vector<std::vector<char>> connectedRegions(regions.size(), std::vector<char>(regions.size(), 0));
        for(int i = 0; i < regions.size(); i++){
            connectedRegions[i][i] = 1;
        }

        connectClosestRegions(regions, connectedRegions);
    }
};