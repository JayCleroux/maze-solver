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

    ~Stack() {
        while (m_first != nullptr) {
            pop();
        }
    }

    void push(int x, int y, char direction) {
        // create a new node
        auto node = new Node();
        node->m_position = std::make_pair(x,y);
        node->m_direction = direction;
        node->m_next = m_first;
        m_first = node;
    }

    std::tuple<int, int, char> pop() {

        // check for empty stack
        if (m_first == nullptr) {
            // maybe throw exception, print message or do nothing....
            return std::make_tuple(-1, -1, ' ');
        }

        // one node exists
        auto node = m_first;
        auto position = node->m_position;
        char direction = node->m_direction;
        m_first = m_first->m_next;
        delete node;
        return std::make_tuple(position.first, position.second, direction);
    }

    char top() {
        // is there a first node?
        if (m_first == nullptr) {
            // return an empty tuple to indicate the stack is empty
            return ' ';
        }
        return m_first->m_direction;
    }

    friend std::ostream& operator<<(std::ostream& output, Stack& stack) {
        auto node = stack.m_first;
        while (node != nullptr) {
            output << node->m_position.first << ", " << node->m_position.second << std::endl;
            node = node->m_next;
        }
        return output;
    }
};



#endif //ASSIGNMENT_2_STACK_H
