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
    bool deleteOrderInMetadata(OrderId orderId, T& mData, Price price);

    template <class T>
    bool modifyOrderMetadata(OrderId orderId, T& mData, Price price, Volume newVolume);

    template <class T>
    std::pair<Price, Volume> fillOrder(T& mData, const OrderType orderType, 
                                    const TranSide tranSide, Volume& orderVolume, 
                                    const Price price, Volume& transactedVolume, Price& totalPrice);

    std::pair<Price, Volume> handleNewOrder(const OrderType orderType, const TranSide tranSide, Volume orderVolume, Price price);

    void addOrder(Side side, Price price, Volume volume);
    bool modifyOrder(OrderId orderId, Volume newVolume);
    bool deleteOrder(OrderId orderId);


    Price getBestPrice(Side side);
};