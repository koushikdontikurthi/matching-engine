#include "OrderBook.h"
#include "ReplayEngine.h"
#include <iostream>
#include <cassert>

void testFullMatch(){
    OrderBook book;

    Order sell;
    sell.id = 1;
    sell.side = Side::Sell;
    sell.type = OrderType::Limit;
    sell.price = 100;
    sell.quantity = 10;
    sell.sequence = 1;
    book.addOrder(sell);

    Order buy;
    buy.id = 2;
    buy.side = Side::Buy;
    buy.type = OrderType::Limit;
    buy.price = 100;
    buy.quantity = 10;
    buy.sequence = 2;
    book.addOrder(buy);

    // After matching, both orders should be fully filled and removed from the order book
    assert(book.getBids().empty());
    assert(book.getAsks().empty());

    std::cout << "testFullMatch passed." << std::endl;
}

void testPartialMatch(){
    OrderBook book;

    Order sell;
    sell.id = 1;
    sell.side = Side::Sell;
    sell.type = OrderType::Limit;
    sell.price = 100;
    sell.quantity = 5;
    sell.sequence = 1;
    book.addOrder(sell);

    Order buy;
    buy.id = 2;
    buy.side = Side::Buy;
    buy.type = OrderType::Limit;
    buy.price = 100;
    buy.quantity = 10; // Partial fill
    buy.sequence = 2;
    book.addOrder(buy);

    // After matching, the sell order should have 5 units remaining
    assert(book.getAsks().empty());
    assert(!book.getBids().empty());
    assert(book.getBids().begin()->second.orders.front().quantity == 5);

    std::cout << "testPartialMatch passed." << std::endl;
}

void testNoMatch(){
    OrderBook book;

    Order sell;
    sell.id = 1;
    sell.side = Side::Sell;
    sell.type = OrderType::Limit;
    sell.price = 100;
    sell.quantity = 10;
    sell.sequence = 1;
    book.addOrder(sell);

    Order buy;
    buy.id = 2;
    buy.side = Side::Buy;
    buy.type = OrderType::Limit;
    buy.price = 90; // No match
    buy.quantity = 10;
    buy.sequence = 2;
    book.addOrder(buy);

    // After adding, both orders should remain in the order book
    assert(!book.getBids().empty());
    assert(!book.getAsks().empty());

    std::cout << "noMatchTest passed." << std::endl;
}

void testCancelOrder(){
    OrderBook book;

    Order sell;
    sell.id = 1;
    sell.side = Side::Sell;
    sell.type = OrderType::Limit;
    sell.price = 100;
    sell.quantity = 10;
    sell.sequence = 1;
    book.addOrder(sell);

    // Cancel the order
    book.cancelOrder(1);

    // After cancellation, the order book should be empty
    assert(book.getAsks().empty());
    assert(book.getBids().empty());

    std::cout << "testCancelOrder passed." << std::endl;
}

void testpricetimePriority(){
    OrderBook book;

    Order sell1;
    sell1.id = 1;
    sell1.side = Side::Sell;
    sell1.type = OrderType::Limit;
    sell1.price = 100;
    sell1.quantity = 5;
    sell1.sequence = 1;
    book.addOrder(sell1);

    Order sell2;
    sell2.id = 2;
    sell2.side = Side::Sell;
    sell2.type = OrderType::Limit;
    sell2.price = 100;
    sell2.quantity = 5;
    sell2.sequence = 2; // Later sequence
    book.addOrder(sell2);

    Order buy;
    buy.id = 3;
    buy.side = Side::Buy;
    buy.type = OrderType::Limit;
    buy.price = 100;
    buy.quantity = 5; // Should fill both buys
    buy.sequence = 3;
    book.addOrder(buy);

    // After matching, both buy orders should be filled in FIFO order
    assert(book.getBids().empty());
    assert(!book.getAsks().empty());
    assert(book.getAsks().begin()->second.orders.front().id == 2);

    std::cout << "testPriceTimePriority passed." << std::endl;
}

void testReplay(){
    std::vector<Event> events;

    // Add a sell order
    Order sell;
    sell.id = 1;
    sell.side = Side::Sell;
    sell.type = OrderType::Limit;
    sell.price = 100;
    sell.quantity = 10;
    sell.sequence = 1;

    events.push_back({EventType::AddOrder, sell, 0});

    // Add a buy order that matches the sell order
    Order buy;
    buy.id = 2;
    buy.side = Side::Buy;
    buy.type = OrderType::Limit;
    buy.price = 100;
    buy.quantity = 5;
    buy.sequence = 2;

    events.push_back({EventType::AddOrder, buy, 0});

    // Cancel the buy order (should have no effect since it's already filled)
    events.push_back({EventType::CancelOrder, {}, 1});
    OrderBook book;
    ReplayEngine engine;
    engine.run(events, book);

    assert(book.getBids().empty());
    assert(book.getAsks().empty());
    std::cout << "testReplay passed." << std::endl;
}
int main(){
    testFullMatch();
    testPartialMatch();
    testNoMatch();
    testCancelOrder();
    testpricetimePriority();
    testReplay();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
