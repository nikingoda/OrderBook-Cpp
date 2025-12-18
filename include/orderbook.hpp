#include <deque>
#include <map>
#include <unordered_map>
#include <memory>
#include "enums.hpp"
#include "order.hpp"

class OrderBook {
private:
    std::map<Price, Volume, std::greater<Price>> mBids;
    std::map<Price, Volume, std::less<Price>> mAsks;
    std::unordered_map<OrderId, std::pair<Side, std::pair<Price, Volume>>> mOrders;
public:
    OrderBook(bool gen_dummies_bids){};

    template <class T>
    void addOrder(T& mData, Price price, Volume volume) {
        auto [it, inserted] = mData.try_emplace(price, volume);
        if(!inserted) {
            it->second+=volume;
        }
    }

    template <class T>
    void deleteOrder(typename T::iterator it, T& mData, Price price, Volume volume) {
        it->second-=volume;
        if(it->second <= 0) {
            mData.erase(it);
        }
    }

    template <class T>
    bool modifyOrder(typename T::iterator it, T& mData, Price price, Volume oldVolume, Volume newVolume) {
        it->second+=(newVolume - oldVolume);
        if(it->second <= 0) {
            mData.erase(it);
        }
    }

    void addOrder(Side side, Price price, Volume volume);
    bool modifyOrder(OrderId orderId, Volume newVolume);
    bool deleteOrder(OrderId orderId);
    Price getBestPrice(Side side);
};