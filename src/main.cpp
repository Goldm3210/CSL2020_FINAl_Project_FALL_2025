#include <iostream>
#include "Maze.h"
#include "MazeGenerator.h"
#include "MazeSolver.h"

int main() {
    std::cout << "=== Maze Generator & Solver ===\n\n";
    
    // Generate a maze
    std::cout << "Generating maze...\n";
    Maze maze = MazeGenerator::generate(21, 21); // Odd dimensions work better
    
    // Display original maze
    std::cout << "\nOriginal Maze:\n";
    maze.print();
    
    // Save maze to file
    maze.saveToFile("maze1.txt");
    std::cout << "\nMaze saved to data/maze1.txt\n";
    
    // Solve the maze
    std::cout << "\nSolving maze using BFS...\n";
    auto solution = MazeSolver::solve(maze, MazeSolver::BFS);
    
    if (!solution.empty()) {
        std::cout << "Solution found! Path length: " << solution.size() << "\n";
        
        // Mark solution path on maze (except start and end)
        for (size_t i = 1; i < solution.size() - 1; i++) {
            auto [x, y] = solution[i];
            maze.setCell(x, y, Maze::SOLUTION);
        }
        
        // Display solved maze
        std::cout << "\nSolved Maze:\n";
        maze.print();
        
        // Save solved maze
        maze.saveToFile("solved_maze1.txt");
        std::cout << "\nSolved maze saved to data/solved_maze1.txt\n";
    } else {
        std::cout << "No solution found!\n";
    }
    
    return 0;
}
