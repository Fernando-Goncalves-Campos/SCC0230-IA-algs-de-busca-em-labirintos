#pragma once

#include <vector>
#include <iterator>
#include <list>
#include <array>
#include <random>
#include <fstream>

class Maze{

public:
enum direction{UP = 1, RIGHT = 2, DOWN = 4, LEFT = 8};
enum paint{WHITE = 16, RED = 32, BLUE = 64, GREEN = 128};

std::vector<std::vector<char>> paths;

Maze(const int size, const int startY, const int startX,
     const int endY, const int endX,
     const int originY = 0, const int originX = 0) : paths(size, std::vector<char>(size, 0)){
    paths[originY][originX] |= paint::WHITE; 
    paths[startY][startX] |= paint::GREEN;
    paths[endY][endX] |= paint::BLUE;
    
    if(size > 1){
        expandMaze(originY, originX);
    }
}

static void mazeToImage(const std::string&& filename, const Maze& mazeObj){
    const std::vector<std::vector<char>> newMaze = mazeObj.placeWalls();
    
    std::fstream fout(filename, std::ofstream::out | std::ofstream::trunc);
    fout << "P3\n" << newMaze.size() << ' ' << newMaze.size() << "\n255\n";

    for (int y = 0; y < newMaze.size(); ++y) {
        for (int x = 0; x < newMaze.size(); ++x) {
            double r = 0;
            double g = 0;
            double b = 0;

            if(newMaze[y][x] & paint::GREEN){
                g = 1;
            }
            else if(newMaze[y][x] & paint::BLUE){
                b = 1;
            }
            else if(newMaze[y][x] & paint::RED){
                r = 1;
            }
            else if(newMaze[y][x] & paint::WHITE){
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

private:
std::vector<std::vector<char>> placeWalls() const {
    int size = paths.size();
    const int newSize = (size<<1)+1;
    std::vector<std::vector<char>> newMaze(newSize, std::vector<char>(newSize, 0));

    for(int y = 0; y < size; y++){
        for(int x = 0; x < size; x++){
            newMaze[(y << 1) + 1][(x << 1) + 1] = paths[y][x];

            if(x && (paths[y][x] & direction::LEFT)){
                newMaze[(y << 1) + 1][x << 1] |= (direction::LEFT | direction::RIGHT | paint::WHITE);

                if( (paths[y][x] & paint::RED) && (paths[y][x-1] & paint::RED) ){
                    newMaze[(y << 1) + 1][x << 1] |= paint::RED;
                }
            }

            if(y && (paths[y][x] & direction::UP)){
                newMaze[y << 1][(x << 1) + 1] |= (direction::UP | direction::DOWN | paint::WHITE);

                if( (paths[y][x] & paint::RED) && (paths[y-1][x] & paint::RED) ){
                    newMaze[y << 1][(x << 1) + 1] |= paint::RED;
                }
            }
        }
    }

    for(int y = 1; y < size; y++){
        for(int x = 0; x < size; x++){
            if(paths[y][x] & direction::UP){
                newMaze[y << 1][(x << 1) + 1] |= (direction::UP | direction::DOWN | paint::WHITE);

                if( (paths[y][x] & paint::RED) && (paths[y-1][x] & paint::RED) ){
                    newMaze[y << 1][(x << 1) + 1] |= paint::RED;
                }
            }
        }
    }
            
    return newMaze;
}

void expandMaze(const int curY, const int curX){
    paths[curY][curX] |= paint::WHITE;
    
    std::list<std::array<int, 3>> walls;
    if(curY != 0) 
        walls.push_back({curY - 1, curX, direction::DOWN});
    if(curX < paths.size()-1) 
        walls.push_back({curY, curX + 1, direction::LEFT});
    if(curY < paths.size()-1) 
        walls.push_back({curY + 1, curX, direction::UP});
    if(curX != 0) 
        walls.push_back({curY, curX - 1, direction::RIGHT});

    std::list<std::array<int, 3>> leftOverWalls;

    int nSelected = 0;
    std::random_device rd;
    while(!walls.empty()){
        const auto curWallIt = std::next(walls.begin(), rd() % walls.size());
        const std::array<int, 3> curWall = *curWallIt;
        walls.erase(curWallIt);

        if(paths[curWall[0]][curWall[1]] & paint::WHITE){
            if(paths[curWall[0]][curWall[1]] & curWall[2]){
                nSelected++;
            }
            else{
                leftOverWalls.push_back(curWall);
            }
            continue;
        }

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

};