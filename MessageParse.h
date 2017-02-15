//
// Created by Chang-Soon Park on 2/11/17.
//

#ifndef ORDERBOOK_MESSAGEPARSE_H
#define ORDERBOOK_MESSAGEPARSE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Message.h"

namespace baselineorderbook {
    class MessageParse {
    public:
        static Message readRow(std::string&& row) {
            std::stringstream ss {row};
            std::string item;
            double time;
            int type;
            long id;
            int qty;
            int price;
            int side;
            char delim = ',';
            std::getline(ss, item, delim);
            std::stringstream(item) >> time;
            std::getline(ss, item, delim);
            std::stringstream(item) >> type;
            std::getline(ss, item, delim);
            std::stringstream(item) >> id;
            std::getline(ss, item, delim);
            std::stringstream(item) >> qty;
            std::getline(ss, item, delim);
            std::stringstream(item) >> price;
            std::getline(ss, item, delim);
            std::stringstream(item) >> side;
            return Message(time, type, id, qty, price, side);
        }

        static std::vector<Message> read(const char *filename) {
            std::ifstream ifs{filename};
            return read(ifs);
        }

        static std::vector<Message> read(std::istream &in) {
            std::vector<Message> orders;
            std::string row;
            while (true) {
                std::getline(in, row);
                if (in.bad() || in.eof()) break;
                auto order = readRow(std::move(row));
                orders.push_back(order);
            }
            return orders;
        }
    };
//    class MessageIterator {
//    public:
//        MessageIterator(std::istream& stream) : stream {stream.good() ? &}
//        static std::vector<Message> read(std::istream& stream) {
//            std::vector<Message> result;
//            std::string line;
//            std::getline(stream, line);
//            std::stringstream lineSteram(line);
//            std::string cell;
//        }
//    };
}

#endif //ORDERBOOK_MESSAGEPARSE_H
