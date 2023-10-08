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

vector<vector<int>> bfs(Maze& maze){
    queue<array<int, 3>> pathsQueue;
    pathsQueue.push({maze.startY, maze.startX, 0});

    vector<vector<int>> solution(maze.paths.size(), vector<int>(maze.paths.size(), -1));

    while(!pathsQueue.empty()){
        const array<int, 3> curPath = pathsQueue.front();
        pathsQueue.pop();

        solution[curPath[0]][curPath[1]] = curPath[2];

        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;        

        if(curPos & Maze::paint::GOAL){
            return solution;
        }

        if((curPos & Maze::direction::UP) && (maze.paths[curPath[0] - 1][curPath[1]] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] - 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Maze::direction::RIGHT) && (maze.paths[curPath[0]][curPath[1] + 1] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] + 1, curPath[2] + 1});
        }
        if((curPos & Maze::direction::DOWN) && (maze.paths[curPath[0] + 1][curPath[1]] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] + 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Maze::direction::LEFT) && (maze.paths[curPath[0]][curPath[1] - 1] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] - 1, curPath[2] + 1});
        }
    }

    return solution;
}

vector<vector<int>> aStar(Maze& maze){
    priority_queue<array<int,4>, vector<array<int, 4>>, bestChoiceComparator> pathsQueue;
    pathsQueue.push({maze.startY, maze.startX, 0, abs(maze.endY - maze.startY) + abs(maze.endX - maze.startX)});

    vector<vector<int>> solution(maze.paths.size(), vector<int>(maze.paths.size(), -1));

    while(!pathsQueue.empty()){
        const array<int, 4> curPath = pathsQueue.top();
        pathsQueue.pop();

        solution[curPath[0]][curPath[1]] = curPath[2];

        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;

        if(curPos & Maze::paint::GOAL){
            return solution;
        }

        if((curPos & Maze::direction::UP) && (maze.paths[curPath[0] - 1][curPath[1]] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] - 1, curPath[1], curPath[2] + 1,
                             curPath[2] + 1 + abs(maze.endY - (curPath[0] - 1)) + abs(maze.endX - curPath[1])});
        }
        if((curPos & Maze::direction::RIGHT) && (maze.paths[curPath[0]][curPath[1] + 1] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] + 1, curPath[2] + 1,
                             curPath[2] + 1 + abs(maze.endY - curPath[0]) + abs(maze.endX - (curPath[1] + 1))});
        }
        if((curPos & Maze::direction::DOWN) && (maze.paths[curPath[0] + 1][curPath[1]] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0] + 1, curPath[1], curPath[2] + 1,
                             curPath[2] + 1 + abs(maze.endY - (curPath[0] + 1)) + abs(maze.endX - curPath[1])});
        }
        if((curPos & Maze::direction::LEFT) && (maze.paths[curPath[0]][curPath[1] - 1] & Maze::paint::VISITED) == 0){
            pathsQueue.push({curPath[0], curPath[1] - 1, curPath[2] + 1,
                             curPath[2] + 1 + abs(maze.endY - curPath[0]) + abs(maze.endX - (curPath[1] - 1))});
        }
    }

    return solution;
}

int main(){
    Maze testMaze(20, 3, 3, 18, 18, 7, 7);
    Maze bfsMaze = testMaze;
    Maze aStarMaze = testMaze;

    vector<vector<int>> solutionAStar = aStar(aStarMaze);
    vector<vector<int>> solutionBFS = bfs(bfsMaze);
    //cout << "A*: " << perftest::benchmark(aStar, aStarMaze) << "ms" << endl;
    //cout << "BFS: " << perftest::benchmark(bfs, bfsMaze) << "ms" << endl;

    //Maze::mazeToImage("unsolvedMaze", testMaze);
    //Maze::mazeToImage("bfsMaze", bfsMaze);
    //Maze::mazeToImage("aStarMaze", aStarMaze);
    //Maze::colorSolution(solutionBFS, bfsMaze);
    //Maze::colorSolution(solutionAStar, aStarMaze);
    //Maze::mazeToImage("bfsMazeSolution", bfsMaze);
    //Maze::mazeToImage("aStarMazeSolution", aStarMaze);

    //cout << "A* to image duration: " << perftest::benchmark(Maze::mazeToImage, "aStarMaze.ppm", aStarMaze) << "ms\n";
    //cout << "BFS to image duration: " << perftest::benchmark(Maze::mazeToImage, "bfsMaze.ppm", bfsMaze) << "ms\n";

    return 0;
}