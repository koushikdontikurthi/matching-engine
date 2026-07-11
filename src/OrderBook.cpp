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

void OrderBook::cancelOrder(uint64_t orderId){
    auto it = orderIndex.find(orderId);
    if(it == orderIndex.end()){
        return;
    }

    Side side = it->second.first;
    int64_t price = it->second.second;
    auto& levels = (side == Side::Buy) ? bids : asks;
    auto levelIt = levels.find(price);
    if(levelIt == levels.end()){
        orderIndex.erase(it);
        return;
    }
    auto& orders = levelIt->second.orders;
    for(auto orderIt = orders.begin(); orderIt != orders.end(); ++orderIt){
        if(orderIt->id == orderId){
            orders.erase(orderIt);
            if(orders.empty()){
                levels.erase(levelIt);
            }
            orderIndex.erase(it);
            return;
        }
    }
} 