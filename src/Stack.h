#ifndef ASSIGNMENT_2_STACK_H
#define ASSIGNMENT_2_STACK_H

#include <iostream>
#include <string>
#include <tuple>

struct Node {
    std::pair<int,int> m_position{};
    Node* m_next {nullptr};
    char m_direction{};
};

class Stack {
private:
    Node* m_first {nullptr};
public:

    ~Stack();

    void push(int x, int y, char direction);

    std::tuple<int, int, char> pop();

    char top();

    friend std::ostream& operator<<(std::ostream& output, Stack& stack);
};

#endif //ASSIGNMENT_2_STACK_H