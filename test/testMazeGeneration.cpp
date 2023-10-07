#include <iostream>
#include <vector>

#include "../src/maze/maze.hpp"

using namespace std;

int main(){
    Maze testMaze(60, 15, 15, 53, 52, 30, 30);
    Maze::mazeToImage("TestMaze.ppm", testMaze);

    return 0;
}