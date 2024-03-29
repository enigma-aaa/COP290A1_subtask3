#pragma once
#include "parser.h"
#include "dateUtil.h"
#include <queue>
#include <iomanip>
#include <cmath>
class DMAPlus
{
public:
    int n;
    int x;
    double p;
    int max_hold_days;
    double c1,c2;

    chrono::year_month_day startDate,endDate,modStartDate;
    string symbolName;

    /*only add trades which have not been balanced*/
    /*not both sellDates and buyDates can have elements 
        recheck to confirm*/
    queue<int> sellDates;
    queue<int> buyDates;

    PriceTable* table;
    CashFlow flow;
    OrderStats stats;

    int curLoc;
    double curPrice;
    chrono::year_month_day curDate;
    double curAbsoluteSum;
    double priceChange_n;
    double smoothingFactor;
    double AMA;
    double ER;
    double curBal;
    int noShares;
    
    DMAPlus(int n,int x,double p,int max_hold_days,
    double c1,double c2,chrono::year_month_day startDate
    ,chrono::year_month_day endDate,string symbolName);
    DMAPlus();
    //first+n is the index of the start function
    void firstPrice(int first);
    void buy();
    void sell();
    int handleMaxHold();
    int check();
    void writeCashFlow();
    void writeCSVfiles();
    void writeFinalPNL();
    void main();
    void multiMain(PriceTable* table);
    void squareOff();
};