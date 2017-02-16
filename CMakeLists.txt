cmake_minimum_required(VERSION 3.5)
project(orderbook)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp OrderBook.cpp OrderBook.h Message.cpp Message.h OpenOrder.cpp OpenOrder.h Chain.cpp Chain.h Node.cpp Node.h MessageParse.cpp MessageParse.h Level.cpp Level.h Logging.cpp Logging.h)
add_executable(orderbook ${SOURCE_FILES})
