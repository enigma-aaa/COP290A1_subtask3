#pragma once
#include "parser.h"
#include "dateUtil.h"
#include <iomanip>
#include <cmath>
class DMA
{
public:
    int n;
    int x;
    double p;

    chrono::year_month_day startDate, endDate, modStartDate;
    string symbolName;

    PriceTable* table;
    CashFlow flow;
    OrderStats stats;
    DateFloat dateFloat;

    double curPrice;
    double cursquaredSum;
    double curSum ;
    double curVar ;
    int noShares;
    double curBal;
    //double curPrice ;
    int curLoc ;
    DMA(int n, int x, double p, chrono::year_month_day startDate, chrono::year_month_day endDate, string symbolName);
    DMA();
    void buy(chrono :: year_month_day date);
    void sell(chrono :: year_month_day date);
    void writeCashFlow(chrono::year_month_day curDate);
    void check();

    void squareOff();
    void writeCSVfiles();
    void writeFinalPNL();
    void writeDebugFiles();
    void main();
    void multiMain(PriceTable* srcTable);
};