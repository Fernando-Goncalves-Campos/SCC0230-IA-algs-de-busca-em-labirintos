#pragma once

#include <vector>
#include <fstream>

class Paths{
public:
    int startY = 0, startX = 0, endY = 0, endX = 0;

    enum direction{UP = 1, RIGHT = 2, DOWN = 4, LEFT = 8};
    enum paint{PATH = 16, VISITED = 32, START = 64, GOAL = 128, SOLUTION = 192};

    std::vector<std::vector<char>> paths;

    Paths(const size_t size, const int startY_, const int startX_,
          const int endY_, const int endX_) 
          : paths(size, std::vector<char>(size, 0)){
        //Saves the start and end positions
        if(startY_ >= 0 && startY_ < size)
            startY = startY_;
        if(startX_ >= 0 && startX_ < size)
            startX = startX_;
        if(endY_ >= 0 && endY_ < size)
            endY = endY_;
        if(endX_ >= 0 && endX_ < size)
            endX = endX_;
    };

    virtual void toImage(const std::string& filename) const {        
        //Creates a .ppm image file in the images folder of the project
        Paths::createImage(filename, paths);
    }

    static void createImage(const std::string& filename, const std::vector<std::vector<char>>& paths){
        //Creates a .ppm image file in the images folder of the project
        std::fstream fout(filename + ".ppm", std::ofstream::out | std::ofstream::trunc);
        fout << "P3\n" << paths.size() << ' ' << paths[0].size() << "\n255\n";

        //Add all the pixels of the image
        for (int y = 0; y < paths.size(); ++y) {
            for (int x = 0; x < paths[0].size(); ++x) {
                double r = 0;
                double g = 0;
                double b = 0;

                if((paths[y][x] & paint::SOLUTION) == paint::SOLUTION){
                    g = b = 1;
                }
                else if(paths[y][x] & paint::START){
                    g = 1;
                }
                else if(paths[y][x] & paint::GOAL){
                    b = 1;
                }
                else if(paths[y][x] & paint::VISITED){
                    r = 1;
                }
                else if(paths[y][x] & paint::PATH){
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

    void colorSolution(const std::vector<std::vector<int>>& solution){
        int curY = endY;
        int curX = endX;
        int curDist = solution[curY][curX];
        
        if(curDist == -1){
            return;
        }

        //Finds the minimun distance path from the start to the goal, starting from the goal
        while(true){
            if((paths[curY][curX] & direction::UP) && (solution[curY-1][curX] == curDist-1)){
                if(--curDist == 0)
                    return;
                paths[--curY][curX] |= paint::SOLUTION;
            }
            else if((paths[curY][curX] & direction::RIGHT) && (solution[curY][curX+1] == curDist-1)){
                if(--curDist == 0)
                    return;
                paths[curY][++curX] |= paint::SOLUTION;
            }
            else if((paths[curY][curX] & direction::DOWN) && (solution[curY+1][curX] == curDist-1)){
                if(--curDist == 0)
                    return;
                paths[++curY][curX] |= paint::SOLUTION;
            }
            else if((paths[curY][curX] & direction::LEFT) && (solution[curY][curX-1] == curDist-1)){
                if(--curDist == 0)
                    return;
                paths[curY][--curX] |= paint::SOLUTION;
            }
            else{
                break;
            }
        }
    }
};