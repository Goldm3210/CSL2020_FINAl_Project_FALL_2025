#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>

class Maze {
private:
    std::vector<std::vector<int>> grid;
    int width, height;
    std::pair<int, int> start;
    std::pair<int, int> end;

public:
    Maze(int w, int h);
    
    // Basic getters and setters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setCell(int x, int y, int value);
    int getCell(int x, int y) const;
    void setStart(int x, int y);
    void setEnd(int x, int y);
    std::pair<int, int> getStart() const { return start; }
    std::pair<int, int> getEnd() const { return end; }
    
    // File operations
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    
    // Display
    void print() const;
    
    // Constants for maze elements
    static const int WALL = 0;
    static const int PATH = 1;
    static const int START = 2;
    static const int END = 3;
    static const int SOLUTION = 4;
};

#endif
