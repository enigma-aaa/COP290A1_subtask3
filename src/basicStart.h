#pragma once
#include "dateUtil.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include "parser.h"
using namespace std;

class Basic{
    public:
        int n,x;
        chrono::year_month_day startDate,endDate,modStartDate;
        string symbolName;

        PriceTable* table;   
        CashFlow flow;
        OrderStats stats;

        double curPrice;
        int noIncDays;
        int noDecDays;
        int noShares;
        double curBal;
        int curLoc;
        int globStartLoc;
    Basic(int n,int x,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName);
    /*first price opver here is not the price at the start 
        date but price n days before the start date*/
    Basic();
    void firstPrice(double orgPrice);
    void nextPrice(double newPrice,chrono::year_month_day newDate);
    void buy(chrono::year_month_day date);
    void sell(chrono::year_month_day date);
    /*Ensure price set to closing price before calling*/
    void squareOff(chrono::year_month_day date);
    void writeCashFlow(chrono::year_month_day date);
    void writeCSVfiles();
    void writeFinalPNL();
    void main();
    void multiMain(PriceTable* srcTable);
};