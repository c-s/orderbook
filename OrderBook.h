//
// Created by Chang-Soon Park on 2/9/17.
//

#ifndef ORDERBOOK_ORDERBOOK_H
#define ORDERBOOK_ORDERBOOK_H

#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <array>
#include "Message.h"
#include "OpenOrder.h"
#include "Chain.h"
#include "Level.h"
#include "Logging.h"

namespace baselineorderbook {

    template<typename PriceToIndex,
             typename IndexToPrice,
             size_t OrderMapSize=0>
    class OrderBook {
    public:
        static Logging& logger() {
            static Logging& log = Logging::getLogger("OrderBook");
            return log;
        };

        OrderBook(int numLevels,
                  PriceToIndex priceToIndex,
                  IndexToPrice indexToPrice)
                : levels(),
                  priceToIndex {priceToIndex},
                  indexToPrice {indexToPrice},
                  bidindex {0}, askindex {numLevels - 1}, bidsize {0}, asksize {0} {
            assert(priceToIndex(indexToPrice(0)) == 0);
            levels.reserve(numLevels);
            for(int i=0;i<numLevels;++i) {
                levels.emplace_back(indexToPrice(i), orderMap);
            }
            if(OrderMapSize > 0) {
                orderMap.reserve(OrderMapSize);
            }
        }

        void updateBidLevelOne(int index) {
            for (int runningIndex = std::max(bidindex, index); runningIndex >= 0; --runningIndex) {
                Level &level = levels[runningIndex];
                if (level.side == 1 && level.qty > 0) {
                    bidindex = runningIndex;
                    bidsize = level.qty;
                    return;
                }
            }
            bidindex = 0;
            bidsize = 0;
        }

        void updateAskLevelOne(int index) {
            for (int runningIndex = std::min(askindex, index); runningIndex < levels.size(); ++runningIndex) {
                Level &level = levels[runningIndex];
                if (level.side == -1 && level.qty > 0) {
                    askindex = runningIndex;
                    asksize = level.qty;
                    return;
                }
            }
            askindex = 0;
            asksize = 0;
        }

        void updateLevelOne(int price, int side) {
            int priceIndex = priceToIndex(price);
            if(side == 1) {
                updateBidLevelOne(priceIndex);
            }
            else {
                updateAskLevelOne(priceIndex);
            }
        }

        void addOrder(OpenOrder order) {
            int index = priceToIndex(order.price);
            auto& level = levels[index];
            auto node = level.addOrder(order);
            orderMap.emplace(order.id, std::move(node));
            updateLevelOne(order.price, order.side);
        }

        void reduceOrder(OpenOrder order) {
            try {
                auto& node = orderMap.at(order.id);
                node.elem.qty -= order.qty;
                if(node.elem.qty == 0) {
                    orderMap.erase(order.id);
                }

                int index = priceToIndex(order.price);
                auto& level = levels[index];
                level.changeQty(-order.qty, order.side);

                updateLevelOne(order.price, order.side);
            }
            catch(std::out_of_range e) {
                logger().error(std::cout) << "redueceOrder: order id " << order.id << " does not exist. ignoring..." << std::endl;
            }
        }

        void deleteOrder(OpenOrder order) {
            int index = priceToIndex(order.price);
            auto& level = levels[index];
            // assuming order.qty is always equal to orderMap[order.id].elem.qty
            int numErased = orderMap.erase(order.id);
            if(numErased > 0) {
                level.changeQty(-order.qty, order.side);
                updateLevelOne(order.price, order.side);
            }
            else {
                logger().error(std::cout) << "deleteOrder: order id " << order.id << " does not exist. ignoring..." << std::endl;
            }
        }

        void processMessage(Message message) {
            OpenOrder openOrder {message};
            switch(message.type) {
                case Message::Type::CREATE: addOrder(openOrder); break;
                case Message::Type::REDUCE:
                case Message::Type::EXECUTE_VISIBLE: reduceOrder(openOrder); break;
                case Message::Type::DELETE: deleteOrder(openOrder); break;
            }
        }

    private:
        std::vector<Level> levels;
        std::unordered_map<long, Node<OpenOrder>> orderMap;
        PriceToIndex priceToIndex;
        IndexToPrice indexToPrice;

        // Let's keep level 1 info.
        int bidindex;
        int askindex;
        int bidsize;
        int asksize;

        friend std::ostream& operator<<(std::ostream& os,
                                        const OrderBook<PriceToIndex, IndexToPrice, OrderMapSize>& orderBook) {
            for (auto level = orderBook.levels.rbegin(); level != orderBook.levels.rend(); ++level) {
                if (!level->orderChain.isEmpty()) {
                    os << *level << std::endl;
                }
            }
            os << "Best Ask: "
               << orderBook.indexToPrice(orderBook.askindex)
               << '(' << orderBook.asksize << ')' << std::endl;
            os << "Best Bid: "
               << orderBook.indexToPrice(orderBook.bidindex)
               << '(' << orderBook.bidsize << ')' << std::endl;
            return os;
        }
    };
}

#endif //ORDERBOOK_ORDERBOOK_H
