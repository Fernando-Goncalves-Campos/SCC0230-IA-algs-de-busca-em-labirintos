#include <iostream>
#include <queue>
#include <array>

#include "../src/maze/maze.hpp"
#include "../src/utils/perftest.hpp"
#include "../src/utils/printtemplates.hpp"

using namespace std;


//Compares which path is the best for A* algorithm
class bestChoiceComparator{
public:
    bool operator()(const array<int, 4>& pointA, const array<int, 4>& pointB){
        return pointA[3] > pointB[3];
    }
};

vector<vector<int>> bfs(Maze& maze){
    //Saves the all the possible next paths to take
    queue<array<int, 3>> pathsQueue;
    pathsQueue.push({maze.startY, maze.startX, 0});

    //Saves the solution to the maze
    vector<vector<int>> solution(maze.paths.size(), vector<int>(maze.paths.size(), -1));

    while(!pathsQueue.empty()){
        //Looks at the next position
        const array<int, 3> curPath = pathsQueue.front();
        pathsQueue.pop();

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;        

        //Checks if found the goal
        if(curPos & Maze::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
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
    //Saves the all the possible next paths to take
    priority_queue<array<int,4>, vector<array<int, 4>>, bestChoiceComparator> pathsQueue;
    pathsQueue.push({maze.startY, maze.startX, 0, abs(maze.endY - maze.startY) + abs(maze.endX - maze.startX)});

    //Saves the solution to the maze
    vector<vector<int>> solution(maze.paths.size(), vector<int>(maze.paths.size(), -1));

    while(!pathsQueue.empty()){
        //Looks at the next position
        const array<int, 4> curPath = pathsQueue.top();
        pathsQueue.pop();

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;

        //Checks if found the goal
        if(curPos & Maze::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
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
    //=======================Image testing=======================
    /*Maze testMaze(20, 3, 3, 18, 18, 7, 7);
    Maze bfsMaze = testMaze;
    Maze aStarMaze = testMaze;

    vector<vector<int>> solutionAStar = aStar(aStarMaze);
    vector<vector<int>> solutionBFS = bfs(bfsMaze);

    Maze::mazeToImage("unsolvedMaze", testMaze);
    Maze::mazeToImage("bfsMaze", bfsMaze);
    Maze::mazeToImage("aStarMaze", aStarMaze);
    Maze::colorSolution(solutionBFS, bfsMaze);
    Maze::colorSolution(solutionAStar, aStarMaze);
    Maze::mazeToImage("bfsMazeSolution", bfsMaze);
    Maze::mazeToImage("aStarMazeSolution", aStarMaze);*/


    //=======================Performance testing=======================
    /*vector<int> sizes{10, 20, 30, 40, 50, 60, 70};
    const int nRepeats = 10;

    /*vector<vector<double>> bfsTimes(sizes.size(), vector<double>(2, 0));
    for(int i = 0; i < sizes.size(); i++){
        const int curSize = sizes[i];
        double totalDuration = 0;
        for(int j = nRepeats; j >= 0; j--){
            clog << "\rMaze size: " << curSize << " Remaining tests: " << j << flush;
            Maze testMaze(curSize, curSize * 0.15, curSize * 0.15, curSize * 0.9, curSize * 0.9, curSize * 0.5, curSize * 0.5);
            totalDuration += perftest::benchmark(bfs, testMaze);
        }
        bfsTimes[i][0] = curSize;
        bfsTimes[i][1] = totalDuration / nRepeats;
    }
    cout << "Bfs results (ms):\n" << bfsTimes << "\n";*/

    /*vector<vector<double>> aStarTimes(sizes.size(), vector<double>(2, 0));
    for(int i = 0; i < sizes.size(); i++){
        const int curSize = sizes[i];
        double totalDuration = 0;
        for(int j = nRepeats; j >= 0; j--){
            clog << "\rMaze size: " << curSize << " Remaining tests: " << j << flush;
            Maze testMaze(curSize, curSize * 0.15, curSize * 0.15, curSize * 0.9, curSize * 0.9, curSize * 0.5, curSize * 0.5);
            totalDuration += perftest::benchmark(aStar, testMaze);
        }
        aStarTimes[i][0] = curSize;
        aStarTimes[i][1] = totalDuration / nRepeats;
    }
    cout << "Bfs results (ms):\n" << aStarTimes << "\n";*/

    return 0;
}