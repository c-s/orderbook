//
// Created by Chang-Soon Park on 2/10/17.
//

#ifndef ORDERBOOK_CHAIN_H
#define ORDERBOOK_CHAIN_H

#include <iostream>
#include <unordered_map>
#include "OpenOrder.h"
#include "Node.h"

namespace baselineorderbook {

    template<typename T>
    class Chain {
    public:
        Chain()
                : head(T(), nullptr, nullptr),
                  tail(T(), nullptr, nullptr) {
            head.next = &tail;
            tail.prev = &head;
        }

        Chain(Chain&& chain)
                : head { std::move(chain.head) },
                  tail { std::move(chain.tail) } {}

        Chain& operator=(Chain&& chain) {
            if(this != &chain) {
                head = std::move(chain.head);
                tail = std::move(chain.tail);
            }
            return *this;
        }

        Node <T> push_forward(T elem) {
            Node <T> *rest = head.next;
            auto newNode = Node<T>(elem, &head, rest);
            head.next = &newNode;
            rest->prev = &newNode;
            return newNode;
        }

        Node<T> push_back(T elem) {
            Node <T> *rest = tail.prev;
            auto newNode = Node<T>(elem, rest, &tail);
            tail.prev = &newNode;
            rest->next = &newNode;
            return newNode;
        }

        bool isEmpty() const {
            return head.next == &tail;
        }

        Node <T> head;
        Node <T> tail;

        friend std::ostream& operator<<(std::ostream& os, const Chain& chain) {
            auto* node = chain.head.next;
            std::string delim = "";
            while(!node->isSentinel()) {
                os << delim;
                os << "[" << node->elem.id << "]";
                os << node->elem.qty * node->elem.side;
                delim = ",";
                node = node->next;
            }
            return os;
        }
    };
}

#endif //ORDERBOOK_CHAIN_H
