#include <deque>
#include <map>
#include <unordered_map>
#include <memory>
#include "enums.hpp"
#include "order.hpp"

class OrderBook {
private:
    //metadata to save orders' informations
    std::map<Price, std::deque<std::unique_ptr<Order>>, std::greater<Price>> mBids;
    std::map<Price, std::deque<std::unique_ptr<Order>>, std::less<Price>> mAsks;
    std::unordered_map<OrderId, std::pair<Side, Price>> mOrders;
public:
    // OrderBook(bool gen_dummies_bids){}; //not use now
    
    template <class T>
    bool deleteOrderInMetadata(OrderId orderId, T& mData, Price price) {
        auto erased = mOrders.erase(orderId);
        if(erased == 0) {
            return false;
        }
        auto& orderListWithPrice = mData[price];
        for(auto it = orderListWithPrice.begin(); it != orderListWithPrice.end(); it++) {
            if((*it)->orderId == orderId) {
                orderListWithPrice.erase(it);
                return true;
            }
        }
        return false;
    }

    template <class T>
    bool modifyOrderMetadata(OrderId orderId, T& mData, Price price, Volume newVolume) {
        for(auto& order:mData[price]) {
            if(order->orderId == orderId) {
                order->volume = newVolume;
                return true;
            }
        }
        return false;
    }

    void addOrder(Side side, Price price, Volume volume);
    bool modifyOrder(OrderId orderId, Volume newVolume);
    bool deleteOrder(OrderId orderId);

    template <class T>
    std::pair<Volume, Price> fillOrder(std::map<Price, std::deque<std::unique_ptr<Order>>, T>& offers,
                                        const OrderType orderType, const TranSide side, Volume& orderVolume,
                                        const Price price, Volume& transactedVolume, Price& totalPrice);
    Price getBestPrice(Side side);
};