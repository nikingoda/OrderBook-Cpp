#include <iostream>

#include "include/enums.hpp"
#include "include/order.hpp"
#include "include/orderbook.hpp"
#include "include/helpers.hpp"

using namespace std;

int main() {
    OrderBook ob;

    cout << "Hello" << endl;

    while(true) {
        int action;
        cout << "Options\n————————————————————\n|1. Print Orderbook|\n|2. Submit order   |\n ————————————————————\nYour choice: ";
		cin >> action;
		cout << "\n";

        if(action == 1) {

        } else {
            int orderTypeInput;
            int tranSideInput;
            Volume volume;
            
            cout << "Enter order type:\n0. Limit order\n1. Market order\nYour choice: ";
			cin >> orderTypeInput;
			OrderType orderType = static_cast<OrderType>(orderTypeInput);

            cout << "\nEnter side:\n0. Buy\n1. Sell\nYour choice: ";
            cin >> tranSideInput;
            TranSide tranSide = static_cast<TranSide>(tranSideInput);

            cout << "\nEnter order volume: ";
            cin >> volume;

            if(orderType == OrderType::Market) {
                cout << "\nSubmitting " << ((tranSide == TranSide::Buy) ? "buying":"selling") 
                    << " market order for " << volume << " units." << "\n";
                
                TimeStamp startTime = getUnixTime();
                std::pair<Price, Volume> report = ob.handleNewOrder(orderType, tranSide, volume);
                TimeStamp endTime = getUnixTime();
                // printReport(report, startTime, endTime);
            } else if (orderType == OrderType::Limit) {
                Price price;
                cout << "\nEnter price: ";
                cin >> price;

                cout << "\nSubmitting " << ((tranSide == TranSide::Buy) ? "buying":"selling") 
                    << " limit order for " << volume << " units." << "\n";

                TimeStamp startTime = getUnixTime();
                std::pair<Price, Volume> report = ob.handleNewOrder(orderType, tranSide, volume, price);
                TimeStamp endTime = getUnixTime();
                // printReport(report, startTime, endTime);
            }
            
        }

        cout << "\n";
    }

    return 0;
}