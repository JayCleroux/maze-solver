#include <iostream>
#include <fstream>
#include "Maze.h"
#include "Stack.h"
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>

Maze::Maze() {
    moves = 0;
    m_x = 0;
    m_y = 0;
    m_grid_size = 25;
    for (auto &i: visited) {
        for (bool &j: i) {
            j = false;
        }
    }
}

void Maze::read_file() {
    std::ifstream file("maze.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            m_lines.push_back(line);
        }
        file.close();
    }
    m_lines[1][0] = '#';
}

std::ostream &operator<<(std::ostream &output, Maze &maze) {

    for (int i = 0; i < (maze.m_grid_size * 2 + 1); i++) {
        for (int j = 0; j < (maze.m_grid_size * 2 + 1); j++) {
            std::cout << maze.m_lines[i][j];
        }
        std::cout << std::endl;
    }

    return output;
}

void Maze::solve_maze() {
    Stack stack;
    bool maze_solved = false;
    m_lines[1][1] = '#';
    while (!maze_solved) {
        //reset_visited();
        //visited[m_x][m_y] = true;
        if (m_x == 24 && m_y == 24) {
            maze_solved = true;
            finish_maze();
        } else {
            look_for_move(stack);
        }
        mark_visited(stack);
        std::cout << stack;
        std::cout << this->moves << std::endl;
        std::cout << *this;
        //visited[m_x][m_y] = true;
    }
}

bool Maze::look_for_move(Stack &stack) {

    std::vector<char> available_moves = get_available_moves();

    while (available_moves.empty()) {
        visited[m_x][m_y] = true;
        char direction = stack.top();
        available_moves.clear();
        move_back_grid_location(direction, stack);
        auto popped = stack.pop();
        m_x = std::get<0>(popped);
        m_y = std::get<1>(popped);
        available_moves = get_available_moves();
        for (auto it = available_moves.begin();
             it != available_moves.end(); ) {
            char move = *it;
            bool remove_move = false;

            if (move == 'S') {
                if (visited[m_x + 1][m_y]) {
                    remove_move = true;
                }
            } else if (move == 'N') {
                if (visited[m_x - 1][m_y]) {
                    remove_move = true;
                }
            } else if (move == 'W') {
                if (visited[m_x][m_y - 1]) {
                    remove_move = true;
                }
            } else if (move == 'E') {
                if (visited[m_x][m_y + 1]) {
                    remove_move = true;
                }
            }

            if (remove_move) {
                it = available_moves.erase(it);
            } else {
                ++it;
            }
        }
        std::cout << *this;
    }

    //Shuffle the vector to select a random direction
    //create random seed using system clock
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(available_moves.begin(), available_moves.end(),
                 std::default_random_engine(seed));

    char move = available_moves[0];
    try_move(move, stack);
    moves++;
    return true;
}

void Maze::move_back_grid_location(char direction, Stack &stack) {
    moves++;
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];
    char &position = m_lines[(m_x * 2) + 1][(m_y * 2) + 1];

    if (direction == 'N') {
        if (m_x < 24) {
            south = ' ';
            position = ' ';
            m_x++;
        }
    } else if (direction == 'S') {
        if (m_x > 0) {
            north = ' ';
            position = ' ';
            m_x--;
        }
    } else if (direction == 'W') {
        if (m_y < 24) {
            position = ' ';
            east = ' ';
            m_y++;
        }
    } else if (direction == 'E') {
        if (m_y > 0) {
            west = ' ';
            position = ' ';
            m_y--;
        }
    }
}

void Maze::finish_maze() {
    char &east = m_lines[m_x * 2 + 1][m_y * 2 + 2];
    char &position = m_lines[m_x * 2 + 1][m_y * 2 + 1];
    position = '#';
    east = '#';
}

void Maze::mark_visited(Stack &stack) {
    char direction = stack.top();
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];
    char &position = m_lines[m_x * 2 + 1][m_y * 2 + 1];

    switch (direction) {
        case 'W':
            if (east != '+' && east != '-' && east != '|') {
                visited[m_x][m_y + 1] = true;
                east = '#';
            }
            break;
        case 'E':
            if (west != '+' && west != '-' && west != '|') {
                visited[m_x][m_y - 1] = true;
                west = '#';
            }
            break;
        case 'S':
            if (north != '+' && north != '-' && north != '|') {
                visited[m_x - 1][m_y] = true;
                north = '#';
            }
            break;
        case 'N':
            if (south != '+' && south != '-' && south != '|') {
                visited[m_x + 1][m_y] = true;
                south = '#';
            }
            break;
        default:
            break;
    }

    if (position != '+' && position != '-' && position != '|') {
        position = '#';
    }
}

bool Maze::move_grid_location(char direction) {
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];
    char &position = m_lines[(m_x * 2) + 1][(m_y * 2) + 1];

    switch (direction) {
        case 'N':
            north = (north != '+' && north != '-' && north != '|') ? '#' : north;
            m_x--;
            break;
        case 'E':
            east = (east != '+' && east != '-' && east != '|') ? '#' : east;
            m_y++;
            break;
        case 'S':
            south = (south != '+' && south != '-' && south != '|') ? '#' : south;
            m_x++;
            break;
        case 'W':
            west = (west != '+' && west != '-' && west != '|') ? '#' : west;
            m_y--;
            break;
        default:
            break;
    }
    return true;
}

bool Maze::try_move(char direction, Stack &stack) {
    int x = m_x;
    int y = m_y;
    switch (direction) {
        case 'W':
            if (y > 0) {
                stack.push(x, y, 'W');
                move_grid_location('W');
                return true;
            }
            break;
        case 'E':
            if (y < m_grid_size - 1) {
                stack.push(x, y, 'E');
                move_grid_location('E');
                return true;
            }
            break;
        case 'N':
            if (x > 0) {
                stack.push(x, y, 'N');
                move_grid_location('N');
                return true;
            }
            break;
        case 'S':
            if (x < m_grid_size - 1) {
                stack.push(x, y, 'S');
                move_grid_location('S');
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

std::vector<char> Maze::get_available_moves() {
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];

    std::vector<char> available_moves;
    if (east == ' ') {
        available_moves.push_back('E');
    }
    if (south == ' ') {
        available_moves.push_back('S');
    }
    if (west == ' ') {
        available_moves.push_back('W');
    }
    if (north == ' ') {
        available_moves.push_back('N');
    }
    return available_moves;
}