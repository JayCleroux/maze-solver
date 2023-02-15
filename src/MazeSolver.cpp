#include <iostream>
#include <fstream>
#include "MazeSolver.h"
#include "Stack.h"
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>

MazeSolver::MazeSolver() {

    m_x = 0;
    m_y = 0;
    m_grid_size = 25;
    // initialize each grid location to unvisited
    for (auto &i: m_visited) {
        for (bool &j: i) {
            j = false;
        }
    }
}

// read in the maze file and store it in the maze object
bool MazeSolver::read_file(const std::string &file_name) {
    std::ifstream file(file_name);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            m_lines.push_back(line);
        }
        file.close();
    } else {
        return false;
    }// mark the entrance square to the left of the initial grid location
    m_lines[1][0] = '#';
    return true;
}

void MazeSolver::solve_maze(const std::string &file_name) {

    Stack stack;
    bool maze_solved = false;
    // initialize the starting grid location
    m_lines[1][1] = '#';
    while (!maze_solved) {
        // if the grid locations are 24 / 24 the maze is complete
        if (m_x == 24 && m_y == 24) {
            std::cout << "Maze solved and saved to output file.";
            maze_solved = true;
            finish_maze();
        } else {
            look_for_move(stack);
        }
        // mark each move as visited
        mark_visited(stack);
    }
    // save the maze to the output file once the maze is completed
    std::ofstream os(file_name);
    os << *this;

}

// function to look for a valid move, will respond to dead ends by looping until
// it finds an available unvisited move
bool MazeSolver::look_for_move(Stack &stack) {

    std::vector<char> available_moves = get_available_moves();

    while (available_moves.empty()) {
        // mark current square as visited
        m_visited[m_x][m_y] = true;
        // get the most recent direction moved from the top of the stack
        char direction = stack.top();
        // clear available moves
        available_moves.clear();
        // move back in that direction
        move_back_grid_location(direction);
        // pop off the last entry in the stack
        auto popped = stack.pop();
        m_x = std::get<0>(popped);
        m_y = std::get<1>(popped);
        // get the available moves
        available_moves = get_available_moves();
        // iterate through the available moves and remove any that lead to a
        // visited square
        for (auto i = available_moves.begin();
             i != available_moves.end();) {
            char move = *i;
            bool remove_move = false;

            if (move == 'S') {
                if (m_visited[m_x + 1][m_y]) {
                    remove_move = true;
                }
            } else if (move == 'N') {
                if (m_visited[m_x - 1][m_y]) {
                    remove_move = true;
                }
            } else if (move == 'W') {
                if (m_visited[m_x][m_y - 1]) {
                    remove_move = true;
                }
            } else if (move == 'E') {
                if (m_visited[m_x][m_y + 1]) {
                    remove_move = true;
                }
            }

            if (remove_move) {
                i = available_moves.erase(i);
            } else {
                ++i;
            }
        }
    }

    //Shuffle the vector to select a random direction
    //create random seed using system clock
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(available_moves.begin(), available_moves.end(),
                 std::default_random_engine(seed));
    // select the first available move from the shuffled vector
    char move = available_moves[0];
    // try the move
    try_move(move, stack);
    return true;
}

// function to move back a grid location for the case of dead ends
void MazeSolver::move_back_grid_location(char direction) {
    // get the values of the characters around the current position
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];
    char &position = m_lines[(m_x * 2) + 1][(m_y * 2) + 1];
    // remove the path of the dead end
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

// function to mark the finishing squares of the maze
void MazeSolver::finish_maze() {

    char &east = m_lines[m_x * 2 + 1][m_y * 2 + 2];
    char &position = m_lines[m_x * 2 + 1][m_y * 2 + 1];
    position = '#';
    east = '#';
}

// function to mark the current grid location on the maze, and mark it as
// visited
void MazeSolver::mark_visited(Stack &stack) {
    // get the most recent direction from the top of the stack
    // as well as the values of the characters around the current position
    char direction = stack.top();
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];
    char &position = m_lines[m_x * 2 + 1][m_y * 2 + 1];

    switch (direction) {
        case 'W':
            if (east != '+' && east != '-' && east != '|') {
                m_visited[m_x][m_y + 1] = true;
                east = '#';
            }
            break;
        case 'E':
            if (west != '+' && west != '-' && west != '|') {
                m_visited[m_x][m_y - 1] = true;
                west = '#';
            }
            break;
        case 'S':
            if (north != '+' && north != '-' && north != '|') {
                m_visited[m_x - 1][m_y] = true;
                north = '#';
            }
            break;
        case 'N':
            if (south != '+' && south != '-' && south != '|') {
                m_visited[m_x + 1][m_y] = true;
                south = '#';
            }
            break;
        default:
            break;
    }
    // mark the current position
    if (position != '+' && position != '-' && position != '|') {
        position = '#';
    }
}

// function to move the grid location on the maze
bool MazeSolver::move_grid_location(char direction) {
    // get the value of the characters around the grid position
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];
    // mark the character between the grid movements with a # and adjust x or y
    switch (direction) {
        case 'N':
            north = '#';
            m_x--;
            break;
        case 'E':
            east = '#';
            m_y++;
            break;
        case 'S':
            south = '#';
            m_x++;
            break;
        case 'W':
            west = '#';
            m_y--;
            break;
        default:
            break;
    }
    return true;
}

// function to take a direction and check if the move is possible
bool MazeSolver::try_move(char direction, Stack &stack) {

    int x = m_x;
    int y = m_y;
    switch (direction) {
        // push the direction moved along with the coordinates onto the stack
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

// function to get the available moves around the current grid location
std::vector<char> MazeSolver::get_available_moves() {
    // get the values of the characters around the current position
    char &north = m_lines[(m_x * 2)][(m_y * 2) + 1];
    char &west = m_lines[(m_x * 2) + 1][m_y * 2];
    char &east = m_lines[(m_x * 2) + 1][(m_y * 2) + 2];
    char &south = m_lines[(m_x * 2) + 2][(m_y * 2) + 1];

    std::vector<char> available_moves;
    // if the character is one space, it is an available move
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

// overloaded output operator to display the maze
std::ofstream &operator<<(std::ofstream &output, MazeSolver &maze) {

    for (int i = 0; i < (maze.m_grid_size * 2 + 1); i++) {
        for (int j = 0; j < (maze.m_grid_size * 2 + 1); j++) {
            output << maze.m_lines[i][j];
        }
        output << std::endl;
    }
    return output;
}