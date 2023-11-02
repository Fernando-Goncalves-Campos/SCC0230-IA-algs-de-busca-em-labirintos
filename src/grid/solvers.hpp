#pragma once

#include <vector>
#include <queue>
#include <stack>
#include <array>
#include <algorithm>

#include "grid.hpp"

//Compares which path is the best for A* algorithm
class bestChoiceComparator{
public:
    bool operator()(const std::array<int, 4>& pointA, const std::array<int, 4>& pointB){
        return pointA[3] > pointB[3];
    }
};

std::vector<std::vector<int>> bfs(Grid& grid){
    //Saves the all the possible next paths to take
    std::queue<std::array<int, 3>> pathsQueue;
    pathsQueue.push({grid.startY, grid.startX, 0});

    //Saves the solution to the grid
    std::vector<std::vector<int>> solution(grid.paths.size(), std::vector<int>(grid.paths.size(), -1));

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsQueue.empty()){
        //Looks at the next position
        const std::array<int, 3> curPath = pathsQueue.front();
        pathsQueue.pop();

        //Tests if the node already was visited
        if(grid.paths[curPath[0]][curPath[1]] & Grid::paint::VISITED){
            continue;
        }

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = grid.paths[curPath[0]][curPath[1]] |= Grid::paint::VISITED;        

        //Creates a frame that will be added to a GIF
        //grid.toImage(std::string("../GIFs/bfsFrames/frame_") + std::to_string(frameCount++));

        //Checks if found the goal
        if(curPos & Grid::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
        if((curPos & Grid::direction::UP) && (grid.paths[curPath[0] - 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] - 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Grid::direction::RIGHT) && (grid.paths[curPath[0]][curPath[1] + 1] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] + 1, curPath[2] + 1});
        }
        if((curPos & Grid::direction::DOWN) && (grid.paths[curPath[0] + 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] + 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Grid::direction::LEFT) && (grid.paths[curPath[0]][curPath[1] - 1] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] - 1, curPath[2] + 1});
        }
    }

    return solution;
}

std::vector<std::vector<int>> aStar(Grid& grid){
    //Saves the all the possible next paths to take
    std::priority_queue<std::array<int,4>, std::vector<std::array<int, 4>>, bestChoiceComparator> pathsQueue;
    pathsQueue.push({grid.startY, grid.startX, 0, abs(grid.endY - grid.startY) + abs(grid.endX - grid.startX)});

    //Saves the solution to the grid
    std::vector<std::vector<int>> solution(grid.paths.size(), std::vector<int>(grid.paths.size(), -1));

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsQueue.empty()){
        //Looks at the next position
        const std::array<int, 4> curPath = pathsQueue.top();
        pathsQueue.pop();

        //Tests if the node already was visited
        if(grid.paths[curPath[0]][curPath[1]] & Grid::paint::VISITED){
            continue;
        }

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = grid.paths[curPath[0]][curPath[1]] |= Grid::paint::VISITED;

        //Creates a frame that will be added to a GIF
        //grid.toImage(std::string("../GIFs/aStarFrames/frame_") + std::to_string(frameCount++));

        //Checks if found the goal
        if(curPos & Grid::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
        if((curPos & Grid::direction::UP) && (grid.paths[curPath[0] - 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] - 1, curPath[1], curPath[2] + 1,
                             curPath[2] + 1 + abs(grid.endY - (curPath[0] - 1)) + abs(grid.endX - curPath[1])});
        }
        if((curPos & Grid::direction::RIGHT) && (grid.paths[curPath[0]][curPath[1] + 1] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] + 1, curPath[2] + 1,
                             curPath[2] + 1 + abs(grid.endY - curPath[0]) + abs(grid.endX - (curPath[1] + 1))});
        }
        if((curPos & Grid::direction::DOWN) && (grid.paths[curPath[0] + 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] + 1, curPath[1], curPath[2] + 1,
                             curPath[2] + 1 + abs(grid.endY - (curPath[0] + 1)) + abs(grid.endX - curPath[1])});
        }
        if((curPos & Grid::direction::LEFT) && (grid.paths[curPath[0]][curPath[1] - 1] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] - 1, curPath[2] + 1,
                             curPath[2] + 1 + abs(grid.endY - curPath[0]) + abs(grid.endX - (curPath[1] - 1))});
        }
    }

    return solution;
}

std::vector<std::vector<int>> dfs(Grid& grid){
    //Saves the all the possible next paths to take
    std::stack<std::array<int, 3>> pathsStack;
    pathsStack.push({grid.startY, grid.startX, 0});

    //Saves the solution to the grid
    std::vector<std::vector<int>> solution(grid.paths.size(), std::vector<int>(grid.paths.size(), -1));

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsStack.empty()){
        //Looks at the next position
        const std::array<int, 3> curPath = pathsStack.top();
        pathsStack.pop();

        //Tests if the node already was visited
        if(grid.paths[curPath[0]][curPath[1]] & Grid::paint::VISITED){
            continue;
        }

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = grid.paths[curPath[0]][curPath[1]] |= Grid::paint::VISITED;        

        //Creates a frame that will be added to a GIF
        //grid.toImage(std::string("../GIFs/dfsFrames/frame_") + std::to_string(frameCount++));

        //Checks if found the goal
        if(curPos & Grid::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
        if((curPos & Grid::direction::RIGHT) && (grid.paths[curPath[0]][curPath[1] + 1] & Grid::paint::VISITED) == 0){
            pathsStack.push({curPath[0], curPath[1] + 1, curPath[2] + 1});
        }
        if((curPos & Grid::direction::DOWN) && (grid.paths[curPath[0] + 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsStack.push({curPath[0] + 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Grid::direction::UP) && (grid.paths[curPath[0] - 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsStack.push({curPath[0] - 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Grid::direction::LEFT) && (grid.paths[curPath[0]][curPath[1] - 1] & Grid::paint::VISITED) == 0){
            pathsStack.push({curPath[0], curPath[1] - 1, curPath[2] + 1});
        }
    }

    return solution;
}

std::vector<std::vector<int>> bestFirst(Grid& grid){
    //Saves the all the possible next paths to take
    std::priority_queue<std::array<int,4>, std::vector<std::array<int, 4>>, bestChoiceComparator> pathsQueue;
    pathsQueue.push({grid.startY, grid.startX, 0, abs(grid.endY - grid.startY) + abs(grid.endX - grid.startX)});

    //Saves the solution to the grid
    std::vector<std::vector<int>> solution(grid.paths.size(), std::vector<int>(grid.paths.size(), -1));

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsQueue.empty()){
        //Looks at the next position
        const std::array<int, 4> curPath = pathsQueue.top();
        pathsQueue.pop();

        //Tests if the node already was visited
        if(grid.paths[curPath[0]][curPath[1]] & Grid::paint::VISITED){
            continue;
        }

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = grid.paths[curPath[0]][curPath[1]] |= Grid::paint::VISITED;

        //Creates a frame that will be added to a GIF
        //grid.toImage(std::string("../GIFs/bestFirstFrames/frame_") + std::to_string(frameCount++));

        //Checks if found the goal
        if(curPos & Grid::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
        if((curPos & Grid::direction::UP) && (grid.paths[curPath[0] - 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] - 1, curPath[1], curPath[2] + 1,
                             abs(grid.endY - (curPath[0] - 1)) + abs(grid.endX - curPath[1])});
        }
        if((curPos & Grid::direction::RIGHT) && (grid.paths[curPath[0]][curPath[1] + 1] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] + 1, curPath[2] + 1,
                             abs(grid.endY - curPath[0]) + abs(grid.endX - (curPath[1] + 1))});
        }
        if((curPos & Grid::direction::DOWN) && (grid.paths[curPath[0] + 1][curPath[1]] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] + 1, curPath[1], curPath[2] + 1,
                             abs(grid.endY - (curPath[0] + 1)) + abs(grid.endX - curPath[1])});
        }
        if((curPos & Grid::direction::LEFT) && (grid.paths[curPath[0]][curPath[1] - 1] & Grid::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] - 1, curPath[2] + 1,
                             abs(grid.endY - curPath[0]) + abs(grid.endX - (curPath[1] - 1))});
        }
    }

    return solution;
}