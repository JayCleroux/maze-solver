#include "MazeSolver.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout
                << "Not enough command line arguments for the program to run.";
        return 1;
    }
    MazeSolver maze;
    std::string in_file_name = (std::string) argv[1];
    std::string out_file_name = (std::string) argv[2];

    if (!maze.read_file(in_file_name)) {
        return 1;
    }
    maze.solve_maze(out_file_name);
    return 0;
}