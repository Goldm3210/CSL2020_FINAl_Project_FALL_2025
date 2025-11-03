#include <iostream>
#include "../src/MazeGenerator.h"
#include "../src/Maze.h"

int main() {
    std::cout << "=== Maze Generator Example ===\n";
    
    // Generate different types of mazes
    Maze maze1 = MazeGenerator::generate(15, 15, MazeGenerator::RECURSIVE_BACKTRACKER);
    std::cout << "Recursive Backtracker Maze:\n";
    maze1.print();
    maze1.saveToFile("maze_recursive.txt");
    
    return 0;
}
