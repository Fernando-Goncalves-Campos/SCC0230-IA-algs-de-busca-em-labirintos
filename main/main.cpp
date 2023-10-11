#include <iostream>
#include <queue>
#include <array>

#include "../src/maze/maze.hpp"
#include "../src/utils/perftest.hpp"
#include "../src/utils/printtemplates.hpp"
#include "../src/utils/createCSV.hpp"

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

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsQueue.empty()){
        //Looks at the next position
        const array<int, 3> curPath = pathsQueue.front();
        pathsQueue.pop();

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;        

        //Creates a frame that will be added to a GIF
        //Maze::mazeToImage(string("../GIFs/bfsFrames/frame_") + to_string(frameCount++), maze);

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

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsQueue.empty()){
        //Looks at the next position
        const array<int, 4> curPath = pathsQueue.top();
        pathsQueue.pop();

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;

        //Creates a frame that will be added to a GIF
        //Maze::mazeToImage(string("../GIFs/aStarFrames/frame_") + to_string(frameCount++), maze);

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

void createExamples(const int size, const int startY_, const int startX_,
                    const int endY_, const int endX_,
                    const int originY = 0, const int originX = 0, const bool allowDeadEnds = false){
    Maze testMaze(size, startY_, startX_, endY_, endX_, originY, originX, allowDeadEnds);
    Maze bfsMaze = testMaze;
    Maze aStarMaze = testMaze;

    //It's possible to create frames to be converted to a gif (this might increase substantially the execution time)
    //Make sure that the lines: 30, 44, 78 and 92 are not commented
    vector<vector<int>> solutionAStar = aStar(aStarMaze);
    vector<vector<int>> solutionBFS = bfs(bfsMaze);


    //Create ppm images to be converted to png files
    /*Maze::mazeToImage("../images/ppmFiles/unsolvedMaze", testMaze);
    Maze::mazeToImage("../images/ppmFiles/bfsMaze", bfsMaze);
    Maze::mazeToImage("../images/ppmFiles/aStarMaze", aStarMaze);
    Maze::colorSolution(solutionBFS, bfsMaze);
    Maze::colorSolution(solutionAStar, aStarMaze);
    Maze::mazeToImage("../images/ppmFiles/bfsMazeSolution", bfsMaze);
    Maze::mazeToImage("../images/ppmFiles/aStarMazeSolution", aStarMaze);*/

    
}

vector<vector<double>> timeMultipleSolves(auto func, const int nRepeats, const vector<int>& sizes){
    vector<vector<double>> times(sizes.size(), vector<double>(2, 0));
    for(int i = 0; i < sizes.size(); i++){
        const int curSize = sizes[i];
        double totalDuration = 0;
        for(int j = nRepeats; j >= 0; j--){
            clog << "\r                                        \r";
            clog << "\rMaze size: " << curSize << " Remaining tests: " << j << flush;
            Maze testMaze(curSize, curSize * 0.15, curSize * 0.15, curSize * 0.9, curSize * 0.9, curSize * 0.5, curSize * 0.5);
            totalDuration += perftest::benchmark(func, testMaze);
        }
        times[i][0] = curSize;
        times[i][1] = totalDuration / nRepeats;
    }
    clog << "\r                                        \r" ;
    return times;
}

int main(){
    //=======================Image testing=============================
    double totalTime = perftest::benchmark(createExamples, 30, 5, 5, 25, 25, 10, 10, false);
    cout << "Total duration for creation of examples: " << totalTime << "ms\n";


    //=======================Performance testing=======================
    //vector<int> sizes{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    //const int nRepeats = 64;

    /*vector<vector<double>> bfsTimes = timeMultipleSolves(bfs, nRepeats, sizes);
    cout << "Bfs results (ms):\n" << bfsTimes << "\n\n";
    createCSV(string("../CSVs/bfsResults"), vector<string>{"Size", "Duration"}, bfsTimes);*/

    /*vector<vector<double>> aStarTimes = timeMultipleSolves(aStar, nRepeats, sizes);
    cout << "A* results (ms):\n" << aStarTimes << "\n\n";
    createCSV(string("../CSVs/aStarResults"), vector<string>{"Size", "Duration"}, aStarTimes);*/

    return 0;
}