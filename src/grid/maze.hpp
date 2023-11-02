#pragma once

#include <vector>
#include <iterator>
#include <list>
#include <array>
#include <random>

#include "grid.hpp"

class Maze : public Grid{

public:
    Maze(const size_t size, const int startY_, const int startX_,
         const int endY_, const int endX_,
         int originY = 0, int originX = 0, const bool allowDeadEnds = false)
         : Grid(size, startY_, startX_, endY_, endX_){
        //Mark the start and goal of the maze
        paths[startY][startX] |= paint::START;
        paths[endY][endX] |= paint::GOAL;

        //Creates a maze the that uses sizeXsize spaces
        if(size == 0)
            return;
        if(originY < 0 || originY > size-1)
            originY = 0;
        
        if(originX < 0 || originX > size-1)
            originX = 0;

        if(allowDeadEnds)
            expandMazeWithDeadEnds(originY, originX);
        else
            expandMaze(originY, originX);

        //Update the positions
        startY = (startY << 1) + 1;
        startX = (startX << 1) + 1;
        endY = (endY << 1) + 1;
        endX = (endX << 1) + 1;
        
        //Add walls to the maze
        placeWalls();
    }
    
private:
    void placeWalls(){
        //Creates a new grid that can hold the walls
        int size = paths.size();
        const int newSize = (size<<1)+1;
        std::vector<std::vector<char>> newPaths(newSize, std::vector<char>(newSize, 0));

        for(int y = 0; y < size; y++){
            for(int x = 0; x < size; x++){
                //Copies the original spaces from the maze
                newPaths[(y << 1) + 1][(x << 1) + 1] = paths[y][x];

                //Adds the colors to horizontal passages in walls
                if(x && (paths[y][x] & direction::LEFT)){
                    newPaths[(y << 1) + 1][x << 1] |= (direction::LEFT | direction::RIGHT | paint::PATH);
                }

                //Adds the colors to vertical passages in walls
                if(y && (paths[y][x] & direction::UP)){
                    newPaths[y << 1][(x << 1) + 1] |= (direction::UP | direction::DOWN | paint::PATH);
                }
            }
        }

        paths = newPaths;
    }

    void expandMaze(const int curY, const int curX){
        //Marks the current position as being part of the maze
        paths[curY][curX] |= paint::PATH;
        
        //Adds the adjacent walls to a list
        std::list<std::array<int, 3>> walls;
        if(curY != 0) 
            walls.push_back({curY - 1, curX, direction::DOWN});
        if(curX < paths[curY].size()-1) 
            walls.push_back({curY, curX + 1, direction::LEFT});
        if(curY < paths.size()-1) 
            walls.push_back({curY + 1, curX, direction::UP});
        if(curX != 0) 
            walls.push_back({curY, curX - 1, direction::RIGHT});

        //Saves wall that stayed in the maze
        std::list<std::array<int, 3>> leftOverWalls;

        int nSelected = 0;
        std::random_device rd;
        while(!walls.empty()){
            //Selects a random wall from the list
            const auto curWallIt = std::next(walls.begin(), rd() % walls.size());
            const std::array<int, 3> curWall = *curWallIt;
            walls.erase(curWallIt);

            //Tests if "opening" the wall will add a new position to the maze
            //In case the next position already is part of the maze, the wall is not "opened"
            if(paths[curWall[0]][curWall[1]] & paint::PATH){
                if(paths[curWall[0]][curWall[1]] & curWall[2])
                    nSelected++;
                else
                    leftOverWalls.push_back(curWall);
                continue;
            }

            //Add the directions to both positions tha can use the opened path
            nSelected++;
            paths[curWall[0]][curWall[1]] |= curWall[2];
            switch(curWall[2]){
                case direction::UP : paths[curY][curX] |= direction::DOWN;
                break;
                case direction::RIGHT : paths[curY][curX] |= direction::LEFT;
                break;
                case direction::DOWN : paths[curY][curX] |= direction::UP;
                break;
                case direction::LEFT : paths[curY][curX] |= direction::RIGHT;
                break;
            }
            expandMaze(curWall[0], curWall[1]);
        }

        //Makes sure that the position isn't a dead end, unless is in the edges of the maze
        if(nSelected == 1 && curX != 0 && curX != paths.size()-1 && curY != 0 && curY != paths.size()-1){
            const std::array<int, 3> curWall = *std::next(leftOverWalls.begin(), rd() % leftOverWalls.size());

            paths[curWall[0]][curWall[1]] |= curWall[2];
            switch(curWall[2]){
                case direction::UP : paths[curY][curX] |= direction::DOWN;
                break;
                case direction::RIGHT : paths[curY][curX] |= direction::LEFT;
                break;
                case direction::DOWN : paths[curY][curX] |= direction::UP;
                break;
                case direction::LEFT : paths[curY][curX] |= direction::RIGHT;
                break;
            }
        }
    }

    void expandMazeWithDeadEnds(const int curY, const int curX){
        //Marks the current position as being part of the maze
        paths[curY][curX] |= paint::PATH;
        
        //Adds the adjacent walls to a list
        std::list<std::array<int, 3>> walls;
        if(curY != 0) 
            walls.push_back({curY - 1, curX, direction::DOWN});
        if(curX < paths.size()-1) 
            walls.push_back({curY, curX + 1, direction::LEFT});
        if(curY < paths.size()-1) 
            walls.push_back({curY + 1, curX, direction::UP});
        if(curX != 0) 
            walls.push_back({curY, curX - 1, direction::RIGHT});

        std::random_device rd;
        while(!walls.empty()){
            //Selects a random wall from the list
            const auto curWallIt = std::next(walls.begin(), rd() % walls.size());
            const std::array<int, 3> curWall = *curWallIt;
            walls.erase(curWallIt);

            //Tests if "opening" the wall will add a new position to the maze
            //In case the next position already is part of the maze, the wall is not "opened"
            if(paths[curWall[0]][curWall[1]] & paint::PATH)
                continue;

            //Add the directions to both positions tha can use the opened path
            paths[curWall[0]][curWall[1]] |= curWall[2];
            switch(curWall[2]){
                case direction::UP : paths[curY][curX] |= direction::DOWN;
                break;
                case direction::RIGHT : paths[curY][curX] |= direction::LEFT;
                break;
                case direction::DOWN : paths[curY][curX] |= direction::UP;
                break;
                case direction::LEFT : paths[curY][curX] |= direction::RIGHT;
                break;
            }
            expandMazeWithDeadEnds(curWall[0], curWall[1]);
        }
    }

};