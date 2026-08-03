#pragma once
#include <vector>
#include <OrderBook.h>

enum class EventType : uint8_t {
    AddOrder,
    CancelOrder
};

struct Event {
    EventType type;
    Order order; // For AddOrder
    uint64_t cancelId; // For CancelOrder
};

class ReplayEngine {
public:
    void run(const std::vector<Event>& events, OrderBook& book);
};