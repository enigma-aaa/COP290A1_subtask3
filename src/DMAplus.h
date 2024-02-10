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
    queue<chrono::year_month_day> sellDates;
    queue<chrono::year_month_day> buyDates;

    PriceTable table;
    CashFlow flow;
    OrderStats stats;

    double curPrice;
    chrono::year_month_day curDate;
    double curAbsoluteSum;
    double priceChange_n;
    double smoothingFactor;
    double AMA;
    double ER;
    double curBal;
    int noShares;
    
    DMAPlus(int n,int x,double p,int max_hold_days,double c1,double c2);
    //first+n is the index of the start function
    void firstPrice(int first);
    void buy();
    void sell();
    void handleMaxHold();
    void check();
    void writeCashFlow();
    void writeCSVfiles();
    void writeFinalPNL();
    void main( );
    void squareOff();
};