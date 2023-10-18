#pragma once

#include <vector>
#include <random>

#include "paths.hpp"

class Cave : public Paths{

public:
    Cave(const size_t size, const int startY_, const int startX_,
         const int endY_, const int endX_, const int fillPercent = 50, const int nIterations = 20)
         : Paths(size, startY_, startX_, endY_, endX_){
        //Creates a maze the that uses sizeXsize spaces
        if(size == 0)
            return;

        fillCave(fillPercent);

        cellAutomata(nIterations);

        createMoves();

        //Mark the start and goal of the maze
        paths[startY][startX] |= paint::START;
        paths[endY][endX] |= paint::GOAL;
    }

private:
    void fillCave(const int fillPercent){
        std::random_device rd;

        for(int i = 0; i < paths.size(); i++){
            for(int j = 0; j < paths[i].size(); j++){
                if(rd() % 101 > fillPercent){
                    paths[i][j] = Paths::paint::PATH;
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
                        paths[i][j] = Paths::paint::PATH;
                    }
                    else if(neighborCount != 4){
                        paths[i][j] = 0;
                    }
                }
            }
        }
    }

    int countNeighbors(const int i, const int j){
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

    void createMoves(){
        for(int i = 0; i < paths.size(); i++){
            for(int j = 0; j < paths[i].size(); j++){
                if(i != 0 && paths[i-1][j] != 0 && paths[i][j] != 0){
                    paths[i-1][j] |= Paths::direction::DOWN;
                    paths[i][j] |= Paths::direction::UP;
                }
                if(j != 0 && paths[i][j-1] != 0 && paths[i][j] != 0){
                    paths[i][j-1] |= Paths::direction::RIGHT;
                    paths[i][j] |= Paths::direction::LEFT;
                }
            }
        }
    }
};