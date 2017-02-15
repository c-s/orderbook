//
// Created by Chang-Soon Park on 2/10/17.
//

#ifndef ORDERBOOK_NODE_H
#define ORDERBOOK_NODE_H

#include "OpenOrder.h"

namespace baselineorderbook {

    template<typename T>
    class Node {
    public:
        Node(T elem, Node<T> *prev, Node<T> *next)
                : elem{elem}, prev{prev}, next{next} {}

        Node(const Node &) = delete;

        Node &operator=(const Node &) = delete;

        Node(Node &&node) : elem{node.elem}, prev{node.prev}, next{node.next} {
            if(node.prev) {
                node.prev->next = this;
                node.prev = nullptr;
            }
            if(node.next) {
                node.next->prev = this;
                node.next = nullptr;
            }
        }

        Node& operator=(Node&& node) {
            if(this != &node) {
                elem = std::move(node.elem);
                prev = node.prev;
                next = node.next;
                if(node.prev) {
                    node.prev->next = this;
                    node.prev = nullptr;
                }
                if(node.next) {
                    node.next->prev = this;
                    node.next = nullptr;
                }
            }
            return *this;
        }

        ~Node() {
            if(prev != nullptr) prev->next = next; // because of the move constructor, need to check nullptr.
            if(next != nullptr) next->prev = prev;
        }

        bool isHeadNode() {
            return prev == nullptr;
        }

        bool isTailNode() {
            return next == nullptr;
        }

        bool isSentinel() { return isHeadNode() || isTailNode(); }

        Node<T> *prev;
        Node<T> *next;

        T elem;
    };

}

#endif //ORDERBOOK_NODE_H
