#include "Maze.h"
#include <iostream>
#include <fstream>

Maze::Maze(int w, int h) : width(w), height(h) {
    grid.resize(height, std::vector<int>(width, WALL));
    start = {1, 1};
    end = {width-2, height-2};
}

void Maze::setCell(int x, int y, int value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        grid[y][x] = value;
    }
}

int Maze::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return WALL;
}

void Maze::setStart(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        start = {x, y};
        setCell(x, y, START);
    }
}

void Maze::setEnd(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        end = {x, y};
        setCell(x, y, END);
    }
}

bool Maze::saveToFile(const std::string& filename) {
    std::ofstream file("data/" + filename);
    if (!file.is_open()) return false;
    
    file << width << " " << height << "\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file << grid[y][x] << " ";
        }
        file << "\n";
    }
    file.close();
    return true;
}

bool Maze::loadFromFile(const std::string& filename) {
    std::ifstream file("data/" + filename);
    if (!file.is_open()) return false;
    
    file >> width >> height;
    grid.resize(height, std::vector<int>(width));
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            file >> grid[y][x];
            if (grid[y][x] == START) start = {x, y};
            if (grid[y][x] == END) end = {x, y};
        }
    }
    file.close();
    return true;
}

void Maze::print() const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch(grid[y][x]) {
                case WALL: std::cout << "█"; break;
                case PATH: std::cout << " "; break;
                case START: std::cout << "S"; break;
                case END: std::cout << "E"; break;
                case SOLUTION: std::cout << "."; break;
                default: std::cout << "?";
            }
        }
        std::cout << "\n";
    }
}
