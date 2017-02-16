//
// Created by Chang-Soon Park on 2/13/17.
//

#ifndef ORDERBOOK_LEVEL_H
#define ORDERBOOK_LEVEL_H

#include <utility>
#include <ostream>
#include "OpenOrder.h"
#include "Chain.h"
#include "Logging.h"

namespace baselineorderbook {
    class Level {
    public:
        static Logging& logger() {
            static Logging& log = Logging::getLogger("Level");
            return log;
        };

        Level(int price, std::unordered_map<long, Node<OpenOrder>>& orderMap) :
                price{price},
                qty {0},
                side {0},
                orderChain {},
                orderMap {orderMap} {}

        Level(Level &&level) : price{level.price},
                               qty {level.qty},
                               side {level.side},
                               orderChain {std::move(level.orderChain)},
                               orderMap {level.orderMap} {}

        Level &operator=(Level &&level) {
            if (this != &level) {
                price = level.price;
                qty = level.qty;
                orderChain = std::move(level.orderChain);
            }
            return *this;
        }

        void changeQty(int chgQty, int newSide) {
            if(side != 0 && side != newSide) {
                // possibly, we have not started from market order,
                // so the side flipped even when qty (hence side) is not 0.
                // so, let's just update the side, and set qty to 0 and clear the order chain.
                qty = 0;
                int count = 0;
                while(!orderChain.isEmpty()) {
                    orderMap.erase(orderChain.head.next->elem.id);
                    ++count;
                }
                logger().error(std::cout) << "order side changed even though cumulative quantity is not 0 at price level "
                                          << price << ". " << count << " open orders removed." << std::endl;
            }
            side = newSide;
            qty += chgQty;
            if(qty == 0) side = 0;
        }

        Node<OpenOrder> addOrder(OpenOrder order) {
            changeQty(order.qty, order.side);
            return orderChain.push_back(order);
        }

        int price;
        int qty;
        int side;
        Chain<OpenOrder> orderChain;
        std::unordered_map<long, Node<OpenOrder>>& orderMap;

        friend std::ostream &operator<<(std::ostream &os, const Level &level) {
            os << level.price << " (";
            os << level.qty << ") <";
            if(level.side == 1) os << "Buy";
            else if (level.side == -1) os << "Sell";
            else os << "Side not initialized yet...";
            os << ">: ";
            os << level.orderChain;
            return os;
        }
    };
}

#endif //ORDERBOOK_LEVEL_H
