#include <iostream>
#include <queue>
#include <array>

#include "../src/maze/maze.hpp"
#include "../src/utils/perftest.hpp"
#include "../src/utils/printtemplates.hpp"

using namespace std;

class bestChoiceComparator{
public:
    bool operator()(const array<int, 4>& pointA, const array<int, 4>& pointB){
        return pointA[3] > pointB[3];
    }
};

void bfs(const int startY, const int startX, const int endY, const int endX, Maze& maze){
    queue<array<int, 3>> pathsQueue;
    pathsQueue.push({startY, startX, 0});

    while(!pathsQueue.empty()){
        const array<int, 3> curPath = pathsQueue.front();
        pathsQueue.pop();

        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::RED;

        if(curPos & Maze::paint::BLUE){
            return;
        }

        if((curPos & Maze::direction::UP) && (maze.paths[curPath[0] - 1][curPath[1]] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0] - 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Maze::direction::RIGHT) && (maze.paths[curPath[0]][curPath[1] + 1] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0], curPath[1] + 1, curPath[2] + 1});
        }
        if((curPos & Maze::direction::DOWN) && (maze.paths[curPath[0] + 1][curPath[1]] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0] + 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Maze::direction::LEFT) && (maze.paths[curPath[0]][curPath[1] - 1] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0], curPath[1] - 1, curPath[2] + 1});
        }
    }
}

void aStar(const int startY, const int startX, const int endY, const int endX, Maze& maze){
    priority_queue<array<int,4>, vector<array<int, 4>>, bestChoiceComparator> pathsQueue;
    pathsQueue.push({startY, startX, 0, abs(endY - startY) + abs(endX - startX)});

    while(!pathsQueue.empty()){
        const array<int, 4> curPath = pathsQueue.top();
        pathsQueue.pop();

        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::RED;

        if(curPos & Maze::paint::BLUE){
            return;
        }

        if((curPos & Maze::direction::UP) && (maze.paths[curPath[0] - 1][curPath[1]] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0] - 1, curPath[1], curPath[2] + 1,
                               curPath[2] + 1 + abs(endY - (curPath[0] - 1)) + abs(endX - curPath[1])});
        }
        if((curPos & Maze::direction::RIGHT) && (maze.paths[curPath[0]][curPath[1] + 1] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0], curPath[1] + 1, curPath[2] + 1,
                               curPath[2] + 1 + abs(endY - curPath[0]) + abs(endX - (curPath[1] + 1))});
        }
        if((curPos & Maze::direction::DOWN) && (maze.paths[curPath[0] + 1][curPath[1]] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0] + 1, curPath[1], curPath[2] + 1,
                               curPath[2] + 1 + abs(endY - (curPath[0] + 1)) + abs(endX - curPath[1])});
        }
        if((curPos & Maze::direction::LEFT) && (maze.paths[curPath[0]][curPath[1] - 1] & Maze::paint::RED) == 0){
            pathsQueue.push({curPath[0], curPath[1] - 1, curPath[2] + 1,
                               curPath[2] + 1 + abs(endY - curPath[0]) + abs(endX - (curPath[1] - 1))});
        }
    }
}

int main(){
    Maze testMaze(70, 15, 15, 50, 50, 30, 30);
    Maze bfsMaze = testMaze;
    Maze aStarMaze = testMaze;

    cout << "A*: " << perftest::benchmark(aStar, 15, 15, 50, 50, aStarMaze) << "ms" << endl;
    cout << "BFS: " << perftest::benchmark(bfs, 15, 15, 50, 50, bfsMaze) << "ms" << endl;

    Maze::mazeToImage("unsolvedMaze.ppm", testMaze);
    Maze::mazeToImage("bfsMaze.ppm", bfsMaze);
    Maze::mazeToImage("aStarMaze.ppm", aStarMaze);

    return 0;
}