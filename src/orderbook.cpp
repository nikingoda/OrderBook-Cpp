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

template <class T>
bool OrderBook::deleteOrderInMetadata(OrderId orderId, T& mData, Price price) {
    auto erased = mOrders.erase(orderId);
    if(erased == 0) {
            return false;
    }
    auto& orderListWithPrice = mData[price];
    for(auto it = orderListWithPrice.begin(); it != orderListWithPrice.end(); ++it) {
        if((*it)->orderId == orderId) {
            orderListWithPrice.erase(it);
            return true;
        }
    }
    return false;
}

template <class T>
bool OrderBook::modifyOrderMetadata(OrderId orderId, T& mData, Price price, Volume newVolume) {
    for(auto& order:mData[price]) {
        if(order->orderId == orderId) {
            order->volume = newVolume;
            return true;
        }
    }
    return false;
}

template <class T>
std::pair<Price, Volume> OrderBook::fillOrder(T& mData, const OrderType orderType, 
                                const TranSide tranSide, Volume& orderVolume, 
                                const Price price, Volume& transactedVolume, Price& totalPrice) {
    for(auto it = mData.begin(); it != mData.end(); ++it) {
        const Price currPrice = it->first;
        auto& ordersList = it->second;
        bool canTransact = true; // always true with market order

        //handle limit order
        if(orderType == OrderType::Limit) { 
            if(tranSide == TranSide::Buy) {
                canTransact = (offerPrice <= price);
            } else if(tranSide == TranSide::Sell) {
                canTransact = (offerPrice >= price);
            }
        }

        if(canTransact) {
            while(!ordersList.empty() && orderVolume > 0) {
                auto& currOrder = ordersList.front();
                const OrderId currOrderId = currOrder->orderId;
                Volume currVolume = currOrder->volume;

                if(currVolume > orderVolume) {
                    transactedVolume += orderVolume;
                    totalPrice += orderVolume * currPrice;
                    currOrder->volume = currVolume - orderVolume;
                    orderVolume = 0;
                } else {
                    transactedVolume += currVolume;
                    totalPrice += currVolume * currPrice;
                    orderVolume -= currVolume;
                    ordersList.pop_front();
                    mOrders.erase(currOrderId);
                }
            }
            if(ordersList.empty()) {
                ordersList.erase(it);
            }
        } else {
            break;
        }
    }
    return std::make_pair(totalPrice, transactedVolume);
}

Price OrderBook::getBestPrice(Side side) {
    if(side == Side::Bid) {
        return mBids.begin()->first;
    } else if(side == Side::Ask) {
        return mAsks.begin()->first;
    }
    return 0;
}