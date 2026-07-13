#include <iostream>
#include "OrderBook.h"

int main(){
    OrderBook book;

    Order sell;
    sell.id = 1;
    sell.side = Side::Sell;
    sell.type = OrderType::Limit;
    sell.price = 143;
    sell.quantity = 10;
    sell.sequence = 1;


    Order buy;
    buy.id = 2;
    buy.side = Side::Buy;
    buy.type = OrderType::Limit;
    buy.price = 145;
    buy.quantity = 5;
    buy.sequence = 2;

    book.addOrder(sell);
    book.addOrder(buy);

    std::cout << "Orders processed successfully." << std::endl;
    return 0;

}