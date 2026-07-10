#include "OrderBook.h"

void OrderBook::addOrder(Order order){
    if (order.side == Side::Buy){
        while (order.quantity > 0 && !asks.empty()){
            auto& bestAsk = asks.begin()->second;
            if(bestAsk.price > order.price){
                break;
            }

            Order& restingOrder = bestAsk.orders.front();

            uint64_t fillQty = std::min(order.quantity, restingOrder.quantity);
            order.quantity -= fillQty;
            restingOrder.quantity -= fillQty;
        
            if(restingOrder.quantity == 0){
                bestAsk.orders.pop_front();
            }

            if(bestAsk.orders.empty()){
                asks.erase(asks.begin());
            }
        
        }
        if(order.quantity > 0 && order.type == OrderType::Limit){
            bids[order.price].price = order.price;
            bids[order.price].orders.push_back(order);
            orderIndex[order.id] = {order.side, order.price};
        }

        
    }else if (order.side == Side::Sell){
        while(order.quantity>0 && !bids.empty()){
            auto& bestBid = bids.begin()->second;
            if(bestBid.price < order.price){
                break;            
            }

            Order& restingOrder = bestBid.orders.front();
            uint64_t fillQty = std::min(order.quantity, restingOrder.quantity);
            order.quantity -= fillQty;
            restingOrder.quantity -= fillQty;

            if(restingOrder.quantity == 0){
                bestBid.orders.pop_front();
            }

            if(bestBid.orders.empty()){
                bids.erase(bids.begin());
            }      
        }
        if(order.quantity>0 && order.type == OrderType::Limit){
            asks[order.price].price = order.price;
            asks[order.price].orders.push_back(order);
            orderIndex[order.id] = {order.side, order.price};
        }
    }
}