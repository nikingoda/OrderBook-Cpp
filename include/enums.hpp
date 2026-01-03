#pragma once

#include <cstdint>

enum class Side { Bid, Ask };
enum class TranSide { Buy, Sell };
enum class OrderType { Limit, Market };

using OrderId = u_int64_t;
using Price = uint64_t;
using Volume = u_int64_t;
using TimeStamp = u_int64_t;