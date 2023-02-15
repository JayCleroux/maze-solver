#include "Maze.h"

int main(int argc, char* argv[]) {

    Maze maze;
    std::string file_name = (std::string)argv[1];
    maze.read_file(file_name);
    maze.solve_maze();
    return 0;
}




