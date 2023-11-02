#pragma once

#include <vector>

#include "grid.hpp"

class Canvas : public Grid{

public:
    Canvas(const size_t size, const int startY_, const int startX_,
         const int endY_, const int endX_)
         : Grid(size, startY_, startX_, endY_, endX_, true){
        //Creates a maze the that uses sizeXsize spaces
        if(size == 0)
            return;

        //Mark the start and goal of the maze
        paths[startY][startX] |= paint::START | paint::PATH;
        paths[endY][endX] |= paint::GOAL | paint::PATH;

        movesFromPaths();
    }
};