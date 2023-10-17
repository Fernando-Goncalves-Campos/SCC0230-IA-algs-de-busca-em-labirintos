#include <iostream>
#include <queue>
#include <stack>
#include <array>
#include <algorithm>

#include "../src/paths/paths.hpp"
#include "../src/paths/maze.hpp"

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
        //maze.mazeToImage(string("../GIFs/bfsFrames/frame_") + to_string(frameCount++));

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
        //maze.mazeToImage(string("../GIFs/aStarFrames/frame_") + to_string(frameCount++));

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

vector<vector<int>> dfs(Maze& maze){
    //Saves the all the possible next paths to take
    stack<array<int, 3>> pathsStack;
    pathsStack.push({maze.startY, maze.startX, 0});

    //Saves the solution to the maze
    vector<vector<int>> solution(maze.paths.size(), vector<int>(maze.paths.size(), -1));

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsStack.empty()){
        //Looks at the next position
        const array<int, 3> curPath = pathsStack.top();
        pathsStack.pop();

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;        

        //Creates a frame that will be added to a GIF
        //maze.mazeToImage(string("../GIFs/dfsFrames/frame_") + to_string(frameCount++));

        //Checks if found the goal
        if(curPos & Maze::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
        if((curPos & Maze::direction::UP) && (maze.paths[curPath[0] - 1][curPath[1]] & Maze::paint::VISITED) == 0){
            pathsStack.push({curPath[0] - 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Maze::direction::RIGHT) && (maze.paths[curPath[0]][curPath[1] + 1] & Maze::paint::VISITED) == 0){
            pathsStack.push({curPath[0], curPath[1] + 1, curPath[2] + 1});
        }
        if((curPos & Maze::direction::DOWN) && (maze.paths[curPath[0] + 1][curPath[1]] & Maze::paint::VISITED) == 0){
            pathsStack.push({curPath[0] + 1, curPath[1], curPath[2] + 1});
        }
        if((curPos & Maze::direction::LEFT) && (maze.paths[curPath[0]][curPath[1] - 1] & Maze::paint::VISITED) == 0){
            pathsStack.push({curPath[0], curPath[1] - 1, curPath[2] + 1});
        }
    }

    return solution;
}

vector<vector<int>> bestFirst(Maze& maze){
    //Saves the all the possible next paths to take
    stack<array<int, 4>> pathsStack;
    pathsStack.push({maze.startY, maze.startX, 0, abs(maze.endY - maze.startY) + abs(maze.endX - maze.startX)});

    //Saves the solution to the maze
    vector<vector<int>> solution(maze.paths.size(), vector<int>(maze.paths.size(), -1));

    //Used for creating gifs
    //int frameCount = 0;

    while(!pathsStack.empty()){
        //Looks at the next position
        const array<int, 4> curPath = pathsStack.top();
        pathsStack.pop();

        //Saves the distance to the current position ftom the start
        solution[curPath[0]][curPath[1]] = curPath[2];

        //Marks current positions as being visited
        char& curPos = maze.paths[curPath[0]][curPath[1]] |= Maze::paint::VISITED;        

        //Creates a frame that will be added to a GIF
        //maze.mazeToImage(string("../GIFs/bestFirstFrames/frame_") + to_string(frameCount++));

        //Checks if found the goal
        if(curPos & Maze::paint::GOAL){
            return solution;
        }

        //Try going to the adjacent spaces
        vector<array<int, 4>> adjSpaces;
        if((curPos & Maze::direction::UP) && (maze.paths[curPath[0] - 1][curPath[1]] & Maze::paint::VISITED) == 0){
            adjSpaces.push_back({curPath[0] - 1, curPath[1], curPath[2] + 1,
            abs(maze.endY - (curPath[0] - 1)) + abs(maze.endX - curPath[1])});
        }
        if((curPos & Maze::direction::RIGHT) && (maze.paths[curPath[0]][curPath[1] + 1] & Maze::paint::VISITED) == 0){
            adjSpaces.push_back({curPath[0], curPath[1] + 1, curPath[2] + 1,
            abs(maze.endY - curPath[0]) + abs(maze.endX - (curPath[1] + 1))});
        }
        if((curPos & Maze::direction::DOWN) && (maze.paths[curPath[0] + 1][curPath[1]] & Maze::paint::VISITED) == 0){
            adjSpaces.push_back({curPath[0] + 1, curPath[1], curPath[2] + 1,
            abs(maze.endY - (curPath[0] + 1)) + abs(maze.endX - curPath[1])});
        }
        if((curPos & Maze::direction::LEFT) && (maze.paths[curPath[0]][curPath[1] - 1] & Maze::paint::VISITED) == 0){
            adjSpaces.push_back({curPath[0], curPath[1] - 1, curPath[2] + 1,
            abs(maze.endY - curPath[0]) + abs(maze.endX - (curPath[1] - 1))});
        }

        //Sorts the adjacent spaces considering their distance to the end
        sort(adjSpaces.begin(), adjSpaces.end(), [](const array<int, 4>& pointA, const array<int, 4>& pointB) -> bool {
            return pointA[3] > pointB[3];
        });

        for(int i = 0; i < adjSpaces.size(); i++){
            pathsStack.push(adjSpaces[i]);
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
    Maze dfsMaze = testMaze;
    Maze bestFirstMaze = testMaze;

    //It's possible to create frames to be converted to a gif (this might increase substantially the execution time)
    //Make sure that the lines for creating frames are not commented in the function
    vector<vector<int>> solutionBfs = bfs(bfsMaze);
    vector<vector<int>> solutionAStar = aStar(aStarMaze);
    vector<vector<int>> solutionDfs = dfs(dfsMaze);
    vector<vector<int>> solutionBestFirst = bestFirst(bestFirstMaze);

    //Create ppm images to be converted to png files
    testMaze.mazeToImage("../images/ppmFiles/unsolvedMaze");
    bfsMaze.mazeToImage("../images/ppmFiles/bfsMaze");
    aStarMaze.mazeToImage("../images/ppmFiles/aStarMaze");
    dfsMaze.mazeToImage("../images/ppmFiles/dfsMaze");
    bestFirstMaze.mazeToImage("../images/ppmFiles/bestFirstMaze");
    bfsMaze.colorSolution(solutionBfs);
    aStarMaze.colorSolution(solutionAStar);
    dfsMaze.colorSolution(solutionDfs);
    bestFirstMaze.colorSolution(solutionBestFirst);
    bfsMaze.mazeToImage("../images/ppmFiles/bfsMazeSolution");
    aStarMaze.mazeToImage("../images/ppmFiles/aStarMazeSolution");
    dfsMaze.mazeToImage("../images/ppmFiles/dfsMazeSolution", solutionDfs);
    bestFirstMaze.mazeToImage("../images/ppmFiles/bestFirstMazeSolution", solutionBestFirst);
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

void main_(){
    //=======================Image testing=============================
    createExamples(30, 5, 5, 25, 25, 10, 10, false);

    //=======================Performance testing=======================
    //vector<int> sizes{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    //const int nRepeats = 64;

    /*vector<vector<double>> bfsTimes = timeMultipleSolves(bfs, nRepeats, sizes);
    cout << "Bfs results (ms):\n" << bfsTimes << "\n\n";
    createCSV(string("../CSVs/bfsResults"), vector<string>{"Size", "Duration"}, bfsTimes);*/

    /*vector<vector<double>> aStarTimes = timeMultipleSolves(aStar, nRepeats, sizes);
    cout << "A* results (ms):\n" << aStarTimes << "\n\n";
    createCSV(string("../CSVs/aStarResults"), vector<string>{"Size", "Duration"}, aStarTimes);*/

    /*vector<vector<double>> dfsTimes = timeMultipleSolves(dfs, nRepeats, sizes);
    cout << "Dfs results (ms):\n" << dfsTimes << "\n\n";
    createCSV(string("../CSVs/dfsResults"), vector<string>{"Size", "Duration"}, dfsTimes);*/

    /*vector<vector<double>> bestFirstTimes = timeMultipleSolves(bestFirst, nRepeats, sizes);
    cout << "Best First results (ms):\n" << bestFirstTimes << "\n\n";
    createCSV(string("../CSVs/bestFirstResults"), vector<string>{"Size", "Duration"}, bestFirstTimes);*/
}

int main(){
    double execTime = perftest::benchmark(main_);
    cout << "Execution time: " << execTime << "ms\n";
}