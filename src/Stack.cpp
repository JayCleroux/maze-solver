#include "Stack.h"

Stack::~Stack() {
    while (m_first != nullptr) {
        pop();
    }
}

std::tuple<int, int, char> Stack::pop() {
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

void Stack::push(int x, int y, char direction)  {
    // create a new node
    auto node = new Node();
    node->m_position = std::make_pair(x,y);
    node->m_direction = direction;
    node->m_next = m_first;
    m_first = node;
}

char Stack::top() {
    // is there a first node?
    if (m_first == nullptr) {
        // return an empty tuple to indicate the stack is empty
        return ' ';
    }
    return m_first->m_direction;
}

std::ostream &operator<<(std::ostream &output, Stack &stack) {
    auto node = stack.m_first;
    while (node != nullptr) {
        output << node->m_position.first <<
        ", " << node->m_position.second << std::endl;
        node = node->m_next;
    }
    return output;
}

