#pragma once

#include <vector>
#include <array>
#include <fstream>

class Grid{
public:
    int startY = 0, startX = 0, endY = 0, endX = 0;

    enum direction{UP = 1, RIGHT = 2, DOWN = 4, LEFT = 8};
    enum paint{PATH = 16, VISITED = 32, START = 64, GOAL = 128, SOLUTION = 192};

    std::vector<std::vector<char>> paths;

    Grid(const size_t size, const int startY_, const int startX_,
          const int endY_, const int endX_, const bool startEmpty = false) 
          : paths(size, std::vector<char>(size, paint::PATH * startEmpty)){
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
        Grid::createImage(filename, paths);
    }

    static void createImage(const std::string& filename, const std::vector<std::vector<char>>& paths){
        //Creates a .ppm image file in the images folder of the project
        std::fstream fout(filename + ".ppm", std::ofstream::out | std::ofstream::trunc);
        fout << "P3\n" << paths.size() << ' ' << paths[0].size() << "\n255\n";

        //Add all the pixels of the image
        for (int y = 0; y < paths.size(); ++y) {
            for (int x = 0; x < paths[y].size(); ++x) {
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
            int nextY = curY;
            int nextX = curX;
            int nextDist = curDist;

            if((paths[curY][curX] & direction::UP) && (solution[curY-1][curX] != -1) && (solution[curY-1][curX] < nextDist)){
                nextDist = solution[curY-1][curX];
                nextY = curY - 1;
            }
            if((paths[curY][curX] & direction::RIGHT) && (solution[curY][curX+1] != -1) && (solution[curY][curX+1] < nextDist)){
                nextDist = solution[curY][curX + 1];
                nextY = curY;
                nextX = curX + 1;
            }
            if((paths[curY][curX] & direction::DOWN) && (solution[curY+1][curX] != -1) && (solution[curY+1][curX] < nextDist)){
                nextDist = solution[curY + 1][curX];
                nextY = curY + 1;
                nextX = curX;
            }
            if((paths[curY][curX] & direction::LEFT) && (solution[curY][curX-1] != -1) && (solution[curY][curX-1] < nextDist)){
                nextDist = solution[curY][curX - 1];
                nextY = curY;
                nextX = curX - 1;
            }
            
            if(nextDist == 0 || curDist == nextDist){
                break;
            }
            else{
                curDist = nextDist;
                curY = nextY;
                curX = nextX;
                paths[curY][curX] |= paint::SOLUTION;
            }
        }
    }

    void movesFromPaths(){
        for(int i = 0; i < paths.size(); i++){
            for(int j = 0; j < paths[i].size(); j++){
                if(i != 0 && paths[i-1][j] != 0 && paths[i][j] != 0){
                    paths[i-1][j] |= Grid::direction::DOWN;
                    paths[i][j] |= Grid::direction::UP;
                }
                if(j != 0 && paths[i][j-1] != 0 && paths[i][j] != 0){
                    paths[i][j-1] |= Grid::direction::RIGHT;
                    paths[i][j] |= Grid::direction::LEFT;
                }
            }
        }
    }
    
    std::vector<std::array<int, 2>> getLine(const int y1, const int x1, const int y2, const int x2) const {
        std::vector<std::array<int, 2>> line;

        int y = y1;
        int x = x1;

        const int dy = y2 - y1;
        const int dx = x2 - x1;

        bool inverted;
        int step = 0;
        int gradientStep = 0;

        int longest = abs(dx);
        int shortest = abs(dy);

        if(longest < shortest){
            inverted = true;
            if(dy < 0){
                step = -1;
            }
            else if(dy > 0){
                step = 1;
            }

            if(dx < 0){
                gradientStep = -1;
            }
            else if(dx > 0){
                gradientStep = 1;
            }

            longest = abs(dy);
            shortest = abs(dx);
        }
        else{
            inverted = false;
            if(dx < 0){
                step = -1;
            }
            else if(dx > 0){
                step = 1;
            }

            if(dy < 0){
                gradientStep = -1;
            }
            else if(dy > 0){
                gradientStep = 1;
            }
        }

        int gradAccumulation = longest >> 1;

        for(int i = 0; i < longest; i++){
            line.push_back({y, x});

            if(inverted){
                y += step;
            }
            else{
                x += step;
            }

            gradAccumulation += shortest;
			if (gradAccumulation >= longest) {
				if (inverted) {
					x += gradientStep;
				}
				else {
					y += gradientStep;
				}
				gradAccumulation -= longest;
			}
        }

        line.push_back({y2, x2});
        return line;
    }

    void paintLine(const int y1, const int x1, const int y2, const int x2, const char paint = 0){
        int y = y1;
        int x = x1;

        const int dy = y2 - y1;
        const int dx = x2 - x1;

        bool inverted;
        int step = 0;
        int gradientStep = 0;

        int longest = abs(dx);
        int shortest = abs(dy);

        if(longest < shortest){
            inverted = true;
            if(dy < 0)
                step = -1;
            else if(dy > 0)
                step = 1;

            if(dx < 0)
                gradientStep = -1;
            else if(dx > 0)
                gradientStep = 1;

            longest = abs(dy);
            shortest = abs(dx);
        }
        else{
            inverted = false;
            if(dx < 0)
                step = -1;
            else if(dx > 0)
                step = 1;

            if(dy < 0)
                gradientStep = -1;
            else if(dy > 0)
                gradientStep = 1;
        }

        int gradAccumulation = longest >> 1;

        for(int i = 0; i < longest; i++){
            paths[y][x] = paint;

            if(inverted){
                y += step;
            }
            else{
                x += step;
            }

            gradAccumulation += shortest;
			if (gradAccumulation >= longest) {
				if (inverted) {
					x += gradientStep;
				}
				else {
					y += gradientStep;
				}
				gradAccumulation -= longest;
			}
        }

        paths[y2][x2] = paint;
    }

    std::vector<std::array<int, 2>> getCircle(const int y0, const int x0, const int radius) const {
        std::vector<std::array<int, 2>> circle;

        const int sqrRadius = radius * radius;
        for (int x = -radius; x <= radius; x++) {
			for (int y = -radius; y <= radius; y++) {
				if (x*x + y*y <= sqrRadius) {
					int posY = y0 + y;
					int posX = x0 + x;
					if (posY >= 0 && posY < paths.size() &&
                        posX >= 0 && posX < paths[posY].size()){
						circle.push_back({posY, posX});
					}
				}
			}
		}

        return circle;
    }

    void paintCircle(const int y0, const int x0, const int radius, const int paint = 0){
        const int sqrRadius = radius * radius;
        for (int x = -radius; x <= radius; x++) {
			for (int y = -radius; y <= radius; y++) {
				if (x*x + y*y <= sqrRadius) {
					int posY = y0 + y;
					int posX = x0 + x;
					if (posY >= 0 && posY < paths.size() &&
                        posX >= 0 && posX < paths[posY].size()){
						paths[posY][posX] = paint;
					}
				}
			}
		}
    }

    void clear(){
        for(int i = 0; i < paths.size(); i++){
            for(int j = 0; j < paths[i].size(); j++){
                paths[i][j] &= 0x1f;
            }
        }
        paths[startY][startX] |= paint::START;
        paths[endY][endX] |= paint::GOAL;
    }

    void placePath(const int y, const int x){
        if(paths[y][x] & paint::PATH){
            return;
        }
        paths[y][x] |= paint::PATH;

        if(y > 0 && paths[y-1][x] & paint::PATH){
            paths[y][x] |= direction::UP;
            paths[y-1][x] |= direction::DOWN;
        }
        if(x < paths[y].size() - 1 && paths[y][x+1] & paint::PATH){
            paths[y][x] |= direction::RIGHT;
            paths[y][x+1] |= direction::LEFT;
        }
        if(y < paths.size() - 1 && paths[y+1][x] & paint::PATH){
            paths[y][x] |= direction::DOWN;
            paths[y+1][x] |= direction::UP;
        }
        if(x > 0 && paths[y][x-1] & paint::PATH){
            paths[y][x] |= direction::LEFT;
            paths[y][x-1] |= direction::RIGHT;
        }
    }

    void placeWall(const int y, const int x){
        if((!(paths[y][x] & paint::PATH)) || (y == startY && x == startX) || (y == endY && x == endX)){
            return;
        }
        paths[y][x] &= ~paint::PATH;

        if(paths[y][x] & direction::UP){
            paths[y][x] &= ~direction::UP;
            paths[y-1][x] &= ~direction::DOWN;
        }
        if(paths[y][x] & direction::RIGHT){
            paths[y][x] &= ~direction::RIGHT;
            paths[y][x+1] &= ~direction::LEFT;
        }
        if(paths[y][x] & direction::DOWN){
            paths[y][x] &= ~direction::DOWN;
            paths[y+1][x] &= ~direction::UP;
        }
        if(paths[y][x] & direction::LEFT){
            paths[y][x] &= ~direction::LEFT;
            paths[y][x-1] &= ~direction::RIGHT;
        }
    }

    void placeStart(const int y, const int x){
        startY = y;
        startX = x;

        clear();
    }

    void placeGoal(const int y, const int x){
        endY = y;
        endX = x;

        clear();
    }

};