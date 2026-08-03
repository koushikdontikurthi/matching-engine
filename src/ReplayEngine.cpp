#include "ReplayEngine.h"

void ReplayEngine::run(const std::vector<Event>& events, OrderBook& book){
    for (const auto& event : events) {
        if (event.type == EventType::AddOrder) {
            book.addOrder(event.order);
        } else if (event.type == EventType::CancelOrder) {
            book.cancelOrder(event.cancelId);
        }
    }
}