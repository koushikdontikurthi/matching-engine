#pragma once
#include <cstdint>

enum class Side: uint8_t {
    Buy, sell
};

enum class OrderType: unit8_t {
    Limit, Market
};

struct Order {
    uint64_t id; //unique identifier
    Side side; //buy or sell
    OrderType type; // limit or market
    int64_t price; // in ticks/cents
    uint64_t quantity; // remaining units to fill
    uint64_t sequence; // monotonic counter for FIFO priority
}