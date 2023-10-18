#include <iostream>
#include <vector>

#include "../src/paths/paths.hpp"
#include "../src/paths/maze.hpp"
#include "../src/paths/solvers.hpp"

#include "../src/utils/perftest.hpp"
#include "../src/utils/printtemplates.hpp"
#include "../src/utils/createCSV.hpp"

using namespace std;

void createExamples(const int size, const int startY_, const int startX_,
                    const int endY_, const int endX_,
                    const int originY = 0, const int originX = 0, const bool allowDeadEnds = false){
    Maze testMaze(size, startY_, startX_, endY_, endX_, originY, originX, allowDeadEnds);
    Maze bfsMaze = testMaze;
    Maze aStarMaze = testMaze;
    Maze dfsMaze = testMaze;
    Maze bestFirstMaze = testMaze;

    //It's possible to create frames to be converted to a gif (this might increase substantially the execution time and create thousands of files)
    //Make sure that the lines for creating frames are not commented in the function
    vector<vector<int>> solutionBfs = bfs(bfsMaze);
    vector<vector<int>> solutionAStar = aStar(aStarMaze);
    vector<vector<int>> solutionDfs = dfs(dfsMaze);
    vector<vector<int>> solutionBestFirst = bestFirst(bestFirstMaze);

    //Create ppm images to be converted to png files
    testMaze.toImage("../images/ppmFiles/unsolvedMaze");
    bfsMaze.toImage("../images/ppmFiles/bfsMaze");
    aStarMaze.toImage("../images/ppmFiles/aStarMaze");
    dfsMaze.toImage("../images/ppmFiles/dfsMaze");
    bestFirstMaze.toImage("../images/ppmFiles/bestFirstMaze");
    bfsMaze.colorSolution(solutionBfs);
    aStarMaze.colorSolution(solutionAStar);
    dfsMaze.colorSolution(solutionDfs);
    bestFirstMaze.colorSolution(solutionBestFirst);
    bfsMaze.toImage("../images/ppmFiles/bfsMazeSolution");
    aStarMaze.toImage("../images/ppmFiles/aStarMazeSolution");
    dfsMaze.toImage("../images/ppmFiles/dfsMazeSolution", solutionDfs);
    bestFirstMaze.toImage("../images/ppmFiles/bestFirstMazeSolution", solutionBestFirst);
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