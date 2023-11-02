#include <iostream>
#include <vector>

#include "../src/grid/grid.hpp"
#include "../src/grid/maze.hpp"
#include "../src/grid/cave.hpp"
#include "../src/grid/solvers.hpp"
#include "../src/grid/canvas.hpp"

#include "../src/utils/perftest.hpp"
#include "../src/utils/printtemplates.hpp"
#include "../src/utils/createCSV.hpp"

using namespace std;

void createExamplesMaze(const size_t size, const int startY_, const int startX_,
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
    dfsMaze.toImage("../images/ppmFiles/dfsMazeSolution");
    bestFirstMaze.toImage("../images/ppmFiles/bestFirstMazeSolution");
}

void createExamplesCave(const size_t size, const int startY_, const int startX_,
                        const int endY_, const int endX_,
                        const int fillPercent = 50, const int nIterations = 20){
    Cave testCave(size, startY_, startX_, endY_, endX_, fillPercent, nIterations);
    Cave bfsCave = testCave;
    Cave aStarCave = testCave;
    Cave dfsCave = testCave;
    Cave bestFirstCave = testCave;

    //It's possible to create frames to be converted to a gif (this might increase substantially the execution time and create thousands of files)
    //Make sure that the lines for creating frames are not commented in the function
    vector<vector<int>> solutionBfs = bfs(bfsCave);
    vector<vector<int>> solutionAStar = aStar(aStarCave);
    vector<vector<int>> solutionDfs = dfs(dfsCave);
    vector<vector<int>> solutionBestFirst = bestFirst(bestFirstCave);

    //Create ppm images to be converted to png files
    testCave.toImage("../images/ppmFiles/unsolvedCave");
    bfsCave.toImage("../images/ppmFiles/bfsCave");
    aStarCave.toImage("../images/ppmFiles/aStarCave");
    dfsCave.toImage("../images/ppmFiles/dfsCave");
    bestFirstCave.toImage("../images/ppmFiles/bestFirstCave");
    bfsCave.colorSolution(solutionBfs);
    aStarCave.colorSolution(solutionAStar);
    dfsCave.colorSolution(solutionDfs);
    bestFirstCave.colorSolution(solutionBestFirst);
    bfsCave.toImage("../images/ppmFiles/bfsCaveSolution");
    aStarCave.toImage("../images/ppmFiles/aStarCaveSolution");
    dfsCave.toImage("../images/ppmFiles/dfsCaveSolution");
    bestFirstCave.toImage("../images/ppmFiles/bestFirstCaveSolution");
}

void createExamplesCanvas(const size_t size, const int startY_, const int startX_,
                          const int endY_, const int endX_){
    Canvas testCanvas(size, startY_, startX_, endY_, endX_);
    testCanvas.placeWall(endY_ - 1, endX_);
    testCanvas.placeWall(endY_, endX_ - 1);
    Canvas bfsCanvas = testCanvas;
    Canvas aStarCanvas = testCanvas;
    Canvas dfsCanvas = testCanvas;
    Canvas bestFirstCanvas = testCanvas;

    //It's possible to create frames to be converted to a gif (this might increase substantially the execution time and create thousands of files)
    //Make sure that the lines for creating frames are not commented in the function
    vector<vector<int>> solutionBfs = bfs(bfsCanvas);
    vector<vector<int>> solutionAStar = aStar(aStarCanvas);
    vector<vector<int>> solutionDfs = dfs(dfsCanvas);
    vector<vector<int>> solutionBestFirst = bestFirst(bestFirstCanvas);

    //Create ppm images to be converted to png files
    testCanvas.toImage("../images/ppmFiles/unsolvedCanvas");
    bfsCanvas.toImage("../images/ppmFiles/bfsCanvas");
    aStarCanvas.toImage("../images/ppmFiles/aStarCanvas");
    dfsCanvas.toImage("../images/ppmFiles/dfsCanvas");
    bestFirstCanvas.toImage("../images/ppmFiles/bestFirstCanvas");
    bfsCanvas.colorSolution(solutionBfs);
    aStarCanvas.colorSolution(solutionAStar);
    dfsCanvas.colorSolution(solutionDfs);
    bestFirstCanvas.colorSolution(solutionBestFirst);
    bfsCanvas.toImage("../images/ppmFiles/bfsCanvasSolution");
    aStarCanvas.toImage("../images/ppmFiles/aStarCanvasSolution");
    dfsCanvas.toImage("../images/ppmFiles/dfsCanvasSolution");
    bestFirstCanvas.toImage("../images/ppmFiles/bestFirstCanvasSolution");
}

vector<vector<double>> timeMultipleSolves(const auto func, const int nRepeats, const vector<int>& sizes){
    vector<vector<double>> times(sizes.size(), vector<double>(2, 0));
    for(int i = 0; i < sizes.size(); i++){
        const int curSize = sizes[i];
        double totalDuration = 0;
        for(int j = nRepeats; j >= 0; j--){
            clog << "\r                                        \r";
            clog << "\rMaze size: " << curSize << " Remaining tests: " << j << flush;
            Maze testMaze(curSize, curSize * 0.35, curSize * 0.35, curSize * 0.65, curSize * 0.65, curSize * 0.5, curSize * 0.5, false);
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
    //createExamplesMaze(20, 5, 5, 15, 15, 10, 10, false);

    //=======================Performance testing=======================
    //vector<int> sizes{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    //const int nRepeats = 128;

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


    //=======================Caves testing=============================
    //createExamplesCave(100, 15, 15, 80, 80, 50, 6);


    //=======================Canvas testing=============================
    //createExamplesCanvas(100, 15, 15, 90, 90);
}

int main(){
    double execTime = perftest::benchmark(main_);
    cout << "Execution time: " << execTime << "ms\n";
}