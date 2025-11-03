#include "MazeSolver.h"
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

std::vector<std::pair<int, int>> MazeSolver::solve(const Maze& maze, Algorithm algo) {
    switch(algo) {
        case DFS:
            return dfsSolve(maze);
        case BFS:
            return bfsSolve(maze);
        case A_STAR:
            return aStarSolve(maze);
        default:
            return bfsSolve(maze);
    }
}

std::vector<std::pair<int, int>> MazeSolver::bfsSolve(const Maze& maze) {
    auto start = maze.getStart();
    auto end = maze.getEnd();
    int width = maze.getWidth();
    int height = maze.getHeight();
    
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::vector<std::vector<std::pair<int, int>>> parent(height, 
        std::vector<std::pair<int, int>>(width, {-1, -1}));
    
    std::queue<std::pair<int, int>> q;
    q.push(start);
    visited[start.second][start.first] = true;
    
    std::vector<std::pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        
        if (x == end.first && y == end.second) {
            // Reconstruct path
            std::vector<std::pair<int, int>> path;
            while (x != -1 && y != -1) {
                path.push_back({x, y});
                auto [px, py] = parent[y][x];
                x = px; y = py;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        for (auto [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                !visited[ny][nx] && maze.getCell(nx, ny) != Maze::WALL) {
                visited[ny][nx] = true;
                parent[ny][nx] = {x, y};
                q.push({nx, ny});
            }
        }
    }
    
    return {}; // No solution found
}

std::vector<std::pair<int, int>> MazeSolver::dfsSolve(const Maze& maze) {
    // Similar to BFS but using stack
    return bfsSolve(maze); // Using BFS for now
}

std::vector<std::pair<int, int>> MazeSolver::aStarSolve(const Maze& maze) {
    // A* algorithm implementation
    return bfsSolve(maze); // Using BFS for simplicity initially
}
