#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include "Maze.h"
#include <vector>

class MazeGenerator {
public:
    enum Algorithm {
        RECURSIVE_BACKTRACKER,
        PRIMS,
        KRUSKALS
    };

    static Maze generate(int width, int height, Algorithm algo = RECURSIVE_BACKTRACKER);

private:
    static Maze recursiveBacktracker(int width, int height);
    static Maze primsAlgorithm(int width, int height);
    static Maze kruskalsAlgorithm(int width, int height);
};

#endif
