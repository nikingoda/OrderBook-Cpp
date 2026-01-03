#include <iostream>

#include "include/helpers.hpp"

using namespace std;

void printReport(pair<Price, Volume> fill, TimeStamp startTime, TimeStamp endTime){
    cout << "\033[33mFilled " << fill.second << "/" << " units @ $" 
        << ((double) fill.second)/fill.first << " average price. Time taken: " 
        << (endTime-startTime) << " nano seconds\033[0m" << "\n";
}