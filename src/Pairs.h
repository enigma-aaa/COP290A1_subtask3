#pragma once 
#include "parser.h"
#include "dateUtil.h"
#include <cmath>
#include <iomanip>
class Pairs{
public:
    int x;
    int n;
    int noShares = 0;
    double threshold;

    string symbol1,symbol2;
    chrono::year_month_day startDate,endDate;
    chrono::year_month_day modStartDate;

    PriceTable* table1;
    PriceTable* table2;
    CashFlow flow;
    OrderStats stats1;
    OrderStats stats2;

    chrono::year_month_day curDate;

    Pairs(int x,int n,double threshold,chrono::year_month_day startDate,
    chrono::year_month_day endDate,string symbol1,string symbol2);
    double curPrice1 = 0;
    double curPrice2 = 0;
    double curSpread = 0;
    double curSum = 0;
    double curMean = 0;
    double curDev = 0;
    double curSqSum = 0;
    double curBal = 0;
    double curZscore = 0;

    void buy();
    void sell();
    void first(int startDateLoc);
    void writeCashFlow(chrono::year_month_day curDate);
    void writeFinalPNL();
    void check();
    void squareOff();
    void writeCSVfiles();
    void main();
};