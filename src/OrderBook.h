#pragma once

#include <map>
#include <unordered_map>
#include <deque>
#include <cstdint>
#include "Order.h"

struct PriceLevel {
    int64_t price;
    std::deque<Order> orders;
};

class OrderBook {
public:
    void addOrder(Order order);
    void cancelOrder(uint64_t orderId);

private:
    std::map<int64_t, PriceLevel, std::greater<int64_t>>bids;
    std::map<int64_t, PriceLevel>asks;
    std::unordered_map<uint64_t, std::pair<Side, int64_t>> orderIndex;

};