//
// Created by Chang-Soon Park on 2/9/17.
//

#ifndef ORDERBOOK_HELPER_H
#define ORDERBOOK_HELPER_H

#include <unordered_map>
#include "Message.h"

namespace baselineorderbook {

    class OpenOrder {
    public:
        OpenOrder(long id, int qty, int price, int side) : id {id}, qty {qty}, price {price}, side {side} {}
        OpenOrder(const Message& order) : id {order.id}, qty {order.qty}, price {order.price}, side {order.side} {}
        OpenOrder() : id {-1L}, qty {-1}, price {0}, side {0} {}

        long id;
        int qty;
        int price;
        int side;

        friend std::ostream& operator<<(std::ostream& os, const OpenOrder& order) {
            os << "id " << order.id << '\t';
            os << "qty " << order.qty << '\t';
            os << "price " << order.price << '\t';
            os << "side " << order.side;
            return os;
        }
    };
}


#endif //ORDERBOOK_HELPER_H
