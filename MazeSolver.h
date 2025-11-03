#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include "Maze.h"
#include <vector>

class MazeSolver {
public:
    enum Algorithm {
        DFS,
        BFS,
        A_STAR
    };

    static std::vector<std::pair<int, int>> solve(const Maze& maze, Algorithm algo = BFS);

private:
    static std::vector<std::pair<int, int>> dfsSolve(const Maze& maze);
    static std::vector<std::pair<int, int>> bfsSolve(const Maze& maze);
    static std::vector<std::pair<int, int>> aStarSolve(const Maze& maze);
};

#endif
