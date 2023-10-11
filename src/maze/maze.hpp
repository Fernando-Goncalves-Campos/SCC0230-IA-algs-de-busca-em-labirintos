#pragma once

#include <vector>
#include <iterator>
#include <list>
#include <array>
#include <random>
#include <fstream>

class Maze{

public:
int startY, startX, endY, endX;
enum direction{UP = 1, RIGHT = 2, DOWN = 4, LEFT = 8};
enum paint{PATH = 16, VISITED = 32, START = 64, GOAL = 128, SOLUTION = 192};

std::vector<std::vector<char>> paths;

Maze(const int size, const int startY_, const int startX_,
     const int endY_, const int endX_,
     const int originY = 0, const int originX = 0, const bool allowDeadEnds = false) : paths(size, std::vector<char>(size, 0)){
    //Mark the start and goal of the maze
    paths[startY_][startX_] |= paint::START;
    paths[endY_][endX_] |= paint::GOAL;

    //Save the start and goal of the maze
    startY = startY_;
    startX = startX_;
    endY = endY_;
    endX = endX_;

    //Creates a maze the that uses sizeXsize spaces
    if(size > 1){
        if(allowDeadEnds){
            expandMazeWithDeadEnds(originY, originX);
        }
        else{
            expandMaze(originY, originX);
        }
    }
}

static void mazeToImage(const std::string&& filename, const Maze& mazeObj){
    //Add walls to the maze to make easier to visualize
    const std::vector<std::vector<char>> newMaze = mazeObj.placeWalls();
    
    //Creates a .ppm image file in the images folder of the projext
    std::fstream fout(filename + ".ppm", std::ofstream::out | std::ofstream::trunc);
    fout << "P3\n" << newMaze.size() << ' ' << newMaze.size() << "\n255\n";

    //Add all the pixels of the image
    for (int y = 0; y < newMaze.size(); ++y) {
        for (int x = 0; x < newMaze.size(); ++x) {
            double r = 0;
            double g = 0;
            double b = 0;

            if((newMaze[y][x] & paint::SOLUTION) == paint::SOLUTION){
                g = b = 1;
            }
            else if(newMaze[y][x] & paint::START){
                g = 1;
            }
            else if(newMaze[y][x] & paint::GOAL){
                b = 1;
            }
            else if(newMaze[y][x] & paint::VISITED){
                r = 1;
            }
            else if(newMaze[y][x] & paint::PATH){
                r = 1;
                g = 1;
                b = 1;
            }

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            fout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    fout.close();
}

static void colorSolution(const std::vector<std::vector<int>>& solution, Maze& maze){
    int curY = maze.endY;
    int curX = maze.endX;
    int curDist = solution[curY][curX];
    
    if(curDist == -1){
        return;
    }

    //Finds the minimun distance path from the start to the goal, starting from the goal
    while(true){
        if((maze.paths[curY][curX] & direction::UP) && (solution[curY-1][curX] == curDist-1)){
            if(--curDist == 0){
                return;
            }
            maze.paths[--curY][curX] |= paint::SOLUTION;
        }
        else if((maze.paths[curY][curX] & direction::RIGHT) && (solution[curY][curX+1] == curDist-1)){
            if(--curDist == 0){
                return;
            }
            maze.paths[curY][++curX] |= paint::SOLUTION;
        }
        else if((maze.paths[curY][curX] & direction::DOWN) && (solution[curY+1][curX] == curDist-1)){
            if(--curDist == 0){
                return;
            }
            maze.paths[++curY][curX] |= paint::SOLUTION;
        }
        else if((maze.paths[curY][curX] & direction::LEFT) && (solution[curY][curX-1] == curDist-1)){
            if(--curDist == 0){
                return;
            }
            maze.paths[curY][--curX] |= paint::SOLUTION;
        }
    }
}

private:
std::vector<std::vector<char>> placeWalls() const {
    //Creates a new grid that can hold the walls
    int size = paths.size();
    const int newSize = (size<<1)+1;
    std::vector<std::vector<char>> newMaze(newSize, std::vector<char>(newSize, 0));

    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++){
            //Copies the original spaces from the maze
            newMaze[(y << 1) + 1][(x << 1) + 1] = paths[y][x];

            //Adds the colors to horizontal passages in walls
            if(x && (paths[y][x] & direction::LEFT)){
                newMaze[(y << 1) + 1][x << 1] |= (direction::LEFT | direction::RIGHT | paint::PATH);

                if( (paths[y][x] & paint::VISITED) && (paths[y][x-1] & paint::VISITED) ){
                    newMaze[(y << 1) + 1][x << 1] |= paint::VISITED;
                }
                if( (paths[y][x] & paint::SOLUTION) && (paths[y][x-1] & paint::SOLUTION) ){
                    newMaze[(y << 1) + 1][x << 1] |= paint::SOLUTION;
                }
            }

            //Adds the colors to vertical passages in walls
            if(y && (paths[y][x] & direction::UP)){
                newMaze[y << 1][(x << 1) + 1] |= (direction::UP | direction::DOWN | paint::PATH);

                if( (paths[y][x] & paint::VISITED) && (paths[y-1][x] & paint::VISITED) ){
                    newMaze[y << 1][(x << 1) + 1] |= paint::VISITED;
                }
                if( (paths[y][x] & paint::SOLUTION) && (paths[y-1][x] & paint::SOLUTION) ){
                    newMaze[y << 1][(x << 1) + 1] |= paint::SOLUTION;
                }
            }
        }
    }

    return newMaze;
}

void expandMaze(const int curY, const int curX){
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
            if(paths[curWall[0]][curWall[1]] & curWall[2]){
                nSelected++;
            }
            else{
                leftOverWalls.push_back(curWall);
            }
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
        if(paths[curWall[0]][curWall[1]] & paint::PATH){
            continue;
        }

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