#include <cstdint>
#include "enums.hpp"
#include "helpers.hpp"

inline OrderId generateUniqueId() {
    static OrderId o_id{0};
    return ++o_id;
}

struct Order{
    OrderId orderId;
    Side side;
    Volume volume;
    Price price;
    TimeStamp timeStamp;

    Order(Side side, Price price, Volume volume, TimeStamp timeStamp = getUnixTime()) 
    : orderId(generateUniqueId()), side(side), volume(volume), price(price), timeStamp(timeStamp) {}
};
