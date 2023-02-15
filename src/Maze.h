#ifndef ASSIGNMENT_2_MAZE_H
#define ASSIGNMENT_2_MAZE_H

#include <string>
#include <iostream>
#include <vector>
#include "Stack.h"


class Maze {
private:
    int moves;
    int m_x, m_y;
    int m_grid_size;
    bool visited[25][25]{};

    std::vector<std::string> m_lines;
public:
    Maze();

    void read_file();

    void solve_maze();

    bool look_for_move(Stack &stack);

    friend std::ostream &operator<<(std::ostream &output, Maze &maze);

    void finish_maze();

    std::vector<char> get_available_moves();

    bool try_move(char direction, Stack &stack);

    bool move_grid_location(char direction);

    void move_back_grid_location(char direction, Stack &stack);

    void mark_visited(Stack &stack);

    void reset_visited();
};

#endif //ASSIGNMENT_2_MAZE_H
