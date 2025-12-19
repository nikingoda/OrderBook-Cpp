#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <map>
#include <thread>
#include <iomanip>
#include <memory>
#include <deque>

#include "../include/order.hpp"
#include "../include/orderbook.hpp"

void OrderBook::addOrder(Side side, Price price, Volume volume) {
    auto order = std::make_unique<Order>(side, price, volume);
    OrderId orderId = order->orderId;
    if(side == Side::Bid) {
        mOrders[orderId] = std::make_pair(Side::Bid, price);
        mBids[orderId].push_back(std::move(order));
    } else {
        mOrders[orderId] = std::make_pair(Side::Ask, price);
        mAsks[orderId].push_back(std::move(order));
    }
}

bool OrderBook::deleteOrder(OrderId orderId) {
    auto [ side, price ] = mOrders[orderId];
    if(side == Side::Bid) {
        return OrderBook::deleteOrderInMetadata(orderId, mBids, price);
    } else if(side == Side::Ask) {
        return OrderBook::deleteOrderInMetadata(orderId, mAsks, price);
    }
    return false;
}

bool OrderBook::modifyOrder(OrderId orderId, Volume newVolume) {
    auto [ side, price ] = mOrders[orderId];
    if(side == Side::Bid) {
        return OrderBook::modifyOrderMetadata(orderId, mBids, price, newVolume);
    } else if(side == Side::Ask) {
        return OrderBook::modifyOrderMetadata(orderId, mAsks, price, newVolume);
    }
    return false;
}

Price OrderBook::getBestPrice(Side side) {
    if(side == Side::Bid) {
        return mBids.begin()->first;
    } else if(side == Side::Ask) {
        return mAsks.begin()->first;
    }
    return 0;
}