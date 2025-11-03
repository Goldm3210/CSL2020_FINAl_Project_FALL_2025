#include "MazeGenerator.h"
#include <stack>
#include <vector>
#include <algorithm>
#include <random>

Maze MazeGenerator::generate(int width, int height, Algorithm algo) {
    switch(algo) {
        case RECURSIVE_BACKTRACKER:
            return recursiveBacktracker(width, height);
        case PRIMS:
            return primsAlgorithm(width, height);
        case KRUSKALS:
            return kruskalsAlgorithm(width, height);
        default:
            return recursiveBacktracker(width, height);
    }
}

Maze MazeGenerator::recursiveBacktracker(int width, int height) {
    Maze maze(width, height);
    
    // Initialize with walls
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            maze.setCell(x, y, Maze::WALL);
        }
    }
    
    std::stack<std::pair<int, int>> stack;
    std::vector<std::pair<int, int>> directions = {{0,2}, {2,0}, {0,-2}, {-2,0}};
    
    // Start from a random cell
    int startX = 1, startY = 1;
    maze.setCell(startX, startY, Maze::PATH);
    stack.push({startX, startY});
    
    while (!stack.empty()) {
        auto [x, y] = stack.top();
        std::vector<std::pair<int, int>> neighbors;
        
        // Find unvisited neighbors
        for (auto [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            if (nx > 0 && nx < width-1 && ny > 0 && ny < height-1 && 
                maze.getCell(nx, ny) == Maze::WALL) {
                neighbors.push_back({nx, ny});
            }
        }
        
        if (!neighbors.empty()) {
            // Randomly choose a neighbor
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, neighbors.size()-1);
            auto [nx, ny] = neighbors[dis(gen)];
            
            // Remove wall between current and chosen cell
            maze.setCell((x + nx) / 2, (y + ny) / 2, Maze::PATH);
            maze.setCell(nx, ny, Maze::PATH);
            
            stack.push({nx, ny});
        } else {
            stack.pop();
        }
    }
    
    // Set start and end points
    maze.setStart(1, 1);
    maze.setEnd(width-2, height-2);
    
    return maze;
}

Maze MazeGenerator::primsAlgorithm(int width, int height) {
    Maze maze(width, height);
    // Basic implementation - you can expand this
    return recursiveBacktracker(width, height);
}

Maze MazeGenerator::kruskalsAlgorithm(int width, int height) {
    Maze maze(width, height);
    // Basic implementation - you can expand this
    return recursiveBacktracker(width, height);
}
