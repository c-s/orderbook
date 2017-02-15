//
// Created by Chang-Soon Park on 2/9/17.
//

#ifndef ORDERBOOK_MESSAGE_H
#define ORDERBOOK_MESSAGE_H

#include <iostream>

namespace baselineorderbook {

    class Message {
    public:
        enum class Type : int {
            CREATE = 1,
            REDUCE = 2,
            DELETE = 3,
            EXECUTE_VISIBLE = 4,
            EXECUTE_HIDDEN = 5,
            TRADING_HALT = 7,
        };

        Message(double time,
              int type,
              long id,
              int qty,
              int price,
              int side) : time {time},
                               type {static_cast<Type>(type)},
                               id {id},
                               qty {qty},
                               price {price},
                               side {side} {}

        double time;
        Type type;
        long id;
        int qty;
        int price;
        int side;

        friend std::ostream& operator<<(std::ostream& os, const Message& message) {
            os << "time " << message.time << '\t';
            os << "type " << static_cast<std::underlying_type<Type>::type>(message.type) << '\t';
            os << "id " << message.id << '\t';
            os << "qty " << message.qty << '\t';
            os << "price " << message.price << '\t';
            os << "side " << message.side;
            return os;
        }
    };
}

#endif //ORDERBOOK_ORDER_H
