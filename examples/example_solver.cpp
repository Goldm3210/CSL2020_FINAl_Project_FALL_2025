/**
 * @file example_solver.cpp
 * @brief Complete demonstration of maze solving algorithms
 * 
 * This example shows how to:
 * 1. Load existing mazes from files
 * 2. Generate new mazes if files don't exist
 * 3. Solve mazes using BFS, DFS, and A* algorithms
 * 4. Compare algorithm performance
 * 5. Display solutions visually
 */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include "../src/Maze.h"
#include "../src/MazeSolver.h"

// Function to print a beautiful header
void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "    " << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

// Function to display algorithm results
void displayAlgorithmResults(const std::string& algorithmName, 
                           const std::vector<std::pair<int, int>>& solution,
                           double timeMs) {
    std::cout << "┌─ " << algorithmName << " " << std::string(40 - algorithmName.length(), '─') << "┐\n";
    std::cout << "│ • Path Length: " << std::setw(8) << solution.size() << " steps" 
              << std::string(24, ' ') << "│\n";
    std::cout << "│ • Time Taken:  " << std::setw(8) << std::fixed << std::setprecision(2) 
              << timeMs << " ms" << std::string(25, ' ') << "│\n";
    
    if (!solution.empty()) {
        std::cout << "│ • Start: (" << std::setw(2) << solution.front().first << ", " 
                  << std::setw(2) << solution.front().second << ")" 
                  << std::string(33, ' ') << "│\n";
        std::cout << "│ • End:   (" << std::setw(2) << solution.back().first << ", " 
                  << std::setw(2) << solution.back().second << ")" 
                  << std::string(33, ' ') << "│\n";
    }
    std::cout << "└" << std::string(54, '─') << "┘\n";
}

// Function to create a simple maze for demonstration
Maze createDemoMaze() {
    std::cout << "   Creating a 15x15 demonstration maze...\n";
    Maze maze(15, 15);
    
    // Initialize all cells as walls first
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 15; x++) {
            maze.setCell(x, y, Maze::WALL);
        }
    }
    
    // Create a simple solvable maze pattern
    int paths[][2] = {
        {1,1}, {2,1}, {3,1}, {4,1}, {5,1}, {6,1}, {7,1}, {8,1}, {9,1}, {10,1}, {11,1}, {12,1}, {13,1},
        {1,2}, {13,2},
        {1,3}, {3,3}, {4,3}, {5,3}, {6,3}, {7,3}, {8,3}, {9,3}, {10,3}, {11,3}, {13,3},
        {1,4}, {3,4}, {13,4},
        {1,5}, {3,5}, {5,5}, {6,5}, {7,5}, {8,5}, {9,5}, {11,5}, {13,5},
        {1,6}, {3,6}, {5,6}, {9,6}, {11,6}, {13,6},
        {1,7}, {3,7}, {5,7}, {6,7}, {7,7}, {9,7}, {11,7}, {13,7},
        {1,8}, {7,8}, {9,8}, {11,8}, {13,8},
        {1,9}, {2,9}, {3,9}, {4,9}, {5,9}, {7,9}, {9,9}, {11,9}, {13,9},
        {1,10}, {5,10}, {7,10}, {9,10}, {11,10}, {13,10},
        {1,11}, {5,11}, {7,11}, {9,11}, {11,11}, {13,11},
        {1,12}, {2,12}, {3,12}, {4,12}, {5,12}, {6,12}, {7,12}, {8,12}, {9,12}, {10,12}, {11,12}, {13,12},
        {1,13}, {13,13}
    };
    
    int pathCount = sizeof(paths) / sizeof(paths[0]);
    for (int i = 0; i < pathCount; i++) {
        maze.setCell(paths[i][0], paths[i][1], Maze::PATH);
    }
    
    // Set start and end points
    maze.setStart(1, 1);
    maze.setEnd(13, 13);
    maze.setCell(1, 1, Maze::START);
    maze.setCell(13, 13, Maze::END);
    
    return maze;
}

int main() {
    printHeader("MAZE SOLVER - COMPLETE DEMONSTRATION");
    
    std::cout << "🎯 Welcome to the Maze Solver Demonstration!\n";
    std::cout << "   This program will show how different algorithms solve mazes.\n\n";
    
    // Step 1: Load or create maze
    printHeader("STEP 1: MAZE SETUP");
    Maze maze(1, 1); // Temporary small maze
    
    std::cout << "📥 Attempting to load maze from file...\n";
    if (maze.loadFromFile("demo_maze.txt")) {
        std::cout << "   ✅ Successfully loaded maze from 'data/demo_maze.txt'\n";
    } else {
        std::cout << "   ⚠️  Could not load maze file.\n";
        std::cout << "   🎨 Generating a demonstration maze...\n";
        maze = createDemoMaze();
        
        // Save the demo maze for future use
        maze.saveToFile("demo_maze.txt");
        std::cout << "   💾 Demo maze saved to 'data/demo_maze.txt'\n";
    }
    
    std::cout << "\n📊 Maze Information:\n";
    std::cout << "   • Size: " << maze.getWidth() << " x " << maze.getHeight() << "\n";
    std::cout << "   • Start: (" << maze.getStart().first << ", " << maze.getStart().second << ")\n";
    std::cout << "   • End: (" << maze.getEnd().first << ", " << maze.getEnd().second << ")\n";
    
    // Display the original maze
    std::cout << "\n🏰 Original Maze Layout:\n";
    maze.print();
    
    // Step 2: Solve with different algorithms
    printHeader("STEP 2: SOLVING THE MAZE");
    
    std::cout << "🔍 We will now solve the maze using 3 different algorithms:\n\n";
    std::cout << "   1. 🚀 BFS (Breadth-First Search) - Guarantees shortest path\n";
    std::cout << "   2. 🌀 DFS (Depth-First Search) - Explores one path deeply\n";
    std::cout << "   3. ⭐ A*  (A-Star) - Smart search using distance estimates\n\n";
    
    // Solve with BFS
    std::cout << "🔹 Solving with BFS Algorithm...\n";
    clock_t startTime = clock();
    std::vector<std::pair<int, int>> bfsSolution = MazeSolver::solve(maze, MazeSolver::BFS);
    double bfsTime = (clock() - startTime) / (double)CLOCKS_PER_SEC * 1000.0;
    displayAlgorithmResults("BFS RESULTS", bfsSolution, bfsTime);
    
    // Solve with DFS
    std::cout << "\n🔹 Solving with DFS Algorithm...\n";
    startTime = clock();
    std::vector<std::pair<int, int>> dfsSolution = MazeSolver::solve(maze, MazeSolver::DFS);
    double dfsTime = (clock() - startTime) / (double)CLOCKS_PER_SEC * 1000.0;
    displayAlgorithmResults("DFS RESULTS", dfsSolution, dfsTime);
    
    // Solve with A*
    std::cout << "\n🔹 Solving with A* Algorithm...\n";
    startTime = clock();
    std::vector<std::pair<int, int>> aStarSolution = MazeSolver::solve(maze, MazeSolver::A_STAR);
    double aStarTime = (clock() - startTime) / (double)CLOCKS_PER_SEC * 1000.0;
    displayAlgorithmResults("A* RESULTS", aStarSolution, aStarTime);
    
    // Step 3: Compare results
    printHeader("STEP 3: ALGORITHM COMPARISON");
    
    std::cout << "📈 Performance Summary:\n\n";
    std::cout << std::left << std::setw(20) << "ALGORITHM" 
              << std::setw(15) << "PATH LENGTH" 
              << std::setw(15) << "TIME (ms)" 
              << std::setw(20) << "NOTES" << "\n";
    
    std::cout << std::string(70, '-') << "\n";
    
    std::cout << std::left << std::setw(20) << "BFS"
              << std::setw(15) << bfsSolution.size()
              << std::setw(15) << std::fixed << std::setprecision(2) << bfsTime
              << std::setw(20) << "✅ Shortest Path" << "\n";
    
    std::cout << std::left << std::setw(20) << "DFS"
              << std::setw(15) << dfsSolution.size()
              << std::setw(15) << std::fixed << std::setprecision(2) << dfsTime
              << std::setw(20) << "⚠️  May be longer" << "\n";
    
    std::cout << std::left << std::setw(20) << "A*"
              << std::setw(15) << aStarSolution.size()
              << std::setw(15) << std::fixed << std::setprecision(2) << aStarTime
              << std::setw(20) << "⚡ Most Efficient" << "\n";
    
    // Step 4: Visualize the best solution
    printHeader("STEP 4: SOLUTION VISUALIZATION");
    
    if (!bfsSolution.empty()) {
        std::cout << "🎨 Displaying maze with BFS solution path:\n\n";
        
        // Create a copy for visualization
        Maze solvedMaze = maze;
        
        // Mark the solution path (skip start and end to keep them visible)
        for (size_t i = 1; i < bfsSolution.size() - 1; i++) {
            auto [x, y] = bfsSolution[i];
            solvedMaze.setCell(x, y, Maze::SOLUTION);
        }
        
        // Display the solved maze
        solvedMaze.print();
        
        // Legend
        std::cout << "\n📖 Legend:\n";
        std::cout << "   █ - Wall        S - Start Point\n";
        std::cout << "   ' ' - Path      E - End Point\n"; 
        std::cout << "   . - Solution Path\n";
        
        // Save the visualized solution
        solvedMaze.saveToFile("solved_demo_maze.txt");
        std::cout << "\n💾 Solved maze saved to 'data/solved_demo_maze.txt'\n";
    } else {
        std::cout << "❌ No solution found! The maze appears to be unsolvable.\n";
    }
    
    // Step 5: Educational insights
    printHeader("STEP 5: KEY INSIGHTS");
    
    std::cout << "🎓 Educational Takeaways:\n\n";
    std::cout << "• BFS (Breadth-First Search):\n";
    std::cout << "  - Explores all neighbors at current depth before moving deeper\n";
    std::cout << "  - Always finds the shortest path in unweighted graphs\n";
    std::cout << "  - Uses more memory but guarantees optimal solution\n\n";
    
    std::cout << "• DFS (Depth-First Search):\n";
    std::cout << "  - Explores as far as possible along each branch before backtracking\n";
    std::cout << "  - May find a solution faster but not necessarily the shortest\n";
    std::cout << "  - Uses less memory but doesn't guarantee optimal path\n\n";
    
    std::cout << "• A* (A-Star Algorithm):\n";
    std::cout << "  - Uses heuristics to estimate distance to goal\n";
    std::cout << "  - Combines actual cost with estimated cost for efficiency\n";
    std::cout << "  - Finds optimal path while being faster than BFS\n";
    
    // Final summary
    printHeader("DEMONSTRATION COMPLETE");
    
    std::cout << "🎉 Successfully demonstrated maze solving with 3 algorithms!\n\n";
    std::cout << "📁 Generated Files:\n";
    std::cout << "   • data/demo_maze.txt - The original maze\n";
    std::cout << "   • data/solved_demo_maze.txt - Maze with solution path\n\n";
    
    std::cout << "🚀 Next Steps:\n";
    std::cout << "   • Modify the maze and run again\n";
    std::cout << "   • Try with larger maze sizes\n";
    std::cout << "   • Implement additional algorithms\n";
    std::cout << "   • Add graphical visualization\n";
    
    std::cout << "\n" << std::string(50, '=') << "\n";
    std::cout << "   Thank you for using Maze Solver! 🎯\n";
    std::cout << std::string(50, '=') << "\n";
    
    return 0;
}
