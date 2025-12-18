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
        mOrders[orderId] = std::make_pair(Side::Bid, std::make_pair(price, volume));
        return OrderBook::addOrder(mBids, price, volume);
    } else {
        mOrders[orderId] = std::make_pair(Side::Ask, std::make_pair(price, volume));
        return OrderBook::addOrder(mAsks, price, volume);
    }
}

bool OrderBook::deleteOrder(OrderId orderId) {
    auto [side, price_and_volume] = mOrders[orderId];
    auto[price, volume] = price_and_volume;
    auto erased = mOrders.erase(orderId);
    if(erased == 0) {
        return false;
    }
    if(side == Side::Bid) {
        auto it = mBids.find(price);
        deleteOrder(it, mBids, price, volume);
    } else {
        auto it = mAsks.find(price);
        deleteOrder(it, mAsks, price, volume);
    }
    return true;
}

bool OrderBook::modifyOrder(OrderId orderId, Volume newVolume) {
    if(mOrders.find(orderId) == mOrders.end()) {
        return false;
    }
    auto [side, price_and_volume] = mOrders[orderId];
    auto[price, oldVolume] = price_and_volume;
    mOrders[orderId].second.second = newVolume;
    if(side == Side::Bid) {
        auto it = mBids.find(price);
        modifyOrder(it, mBids, price, oldVolume, newVolume);
    } else {
        auto it = mAsks.find(price);
        modifyOrder(it, mAsks, price, oldVolume, newVolume);
    }
    return true;
}

Price OrderBook::getBestPrice(Side side) {
    if(side == Side::Bid) {
        return mBids.begin()->first;
    } else if(side == Side::Ask) {
        return mAsks.begin()->first;
    }
    return 0;
}