#include <chrono>
#include <iostream>
#include "OrderBook.h"
#include "MessageParse.h"
#include "Logging.h"

using namespace baselineorderbook;

std::chrono::milliseconds timeit(std::function<void()> func) {
    using Clock = std::chrono::steady_clock;
    std::chrono::time_point<Clock> start = Clock::now();
    func();
    std::chrono::time_point<Clock> end = Clock::now();
    std::chrono::milliseconds diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return diff;
}

void test() {
    auto priceToIndex = [](int price) -> int { return price / 100; };
    auto indexToPrice = [](int index) -> int { return index * 100; };
    OrderBook<decltype(priceToIndex), decltype(indexToPrice)>
            orderBook(100000, priceToIndex, indexToPrice);

    auto msft_messages_10 = "/Users/changsoonpark/Downloads/LOBSTER_SampleFile_MSFT_2012-06-21_10/MSFT_2012-06-21_34200000_57600000_message_10.csv";
    auto msft_messages_50 = "/Users/changsoonpark/Downloads/LOBSTER_SampleFile_MSFT_2012-06-21_50/MSFT_2012-06-21_34200000_37800000_message_50.csv";
    auto aapl_messages_50 = "/Users/changsoonpark/Downloads/LOBSTER_SampleFile_AAPL_2012-06-21_50/AAPL_2012-06-21_34200000_37800000_message_50.csv";
    auto messages = MessageParse::read(aapl_messages_50);
    auto time = timeit([&]() {
        for(const auto& message : messages) {
            orderBook.processMessage(message);
        }
    });
    std::cout << orderBook;
    std::cout << "time elapsed processing "
              << messages.size() << " messages: "
              << time.count() << "ms"
              << std::endl;
}

void testlog() {
    auto& logger1 = SimpleLogging::getLogger(Logging::LogLevel::DEBUG, "test1");
    auto& logger2 = SimpleLogging::getLogger("test2");

    logger1.debug("logger1 debug");
    logger1.info("logger1 info");
    logger1.warning("logger1 warning");
    logger1.error("logger1 error");

    logger2.debug("logger2 debug");
    logger2.info("logger2 info");
    logger2.warning("logger2 warning");
    logger2.error("logger2 error");

    SimpleLogging::globalLogLevel() = Logging::LogLevel::ERROR;

    logger2.debug("2 logger2 debug");
    logger2.info("2 logger2 info");
    logger2.warning("2 logger2 warning");
    logger2.error("2 logger2 error");
}
int main() {
    test();
    return 0;
}