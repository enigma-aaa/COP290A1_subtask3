#include "parser.h"
#include "dateUtil.h"
#include <cmath>
#include <iomanip>
class ADXStrat{
public:
    int x;
    int n;
    int noShares;
    double adx_threshold;

    chrono::year_month_day startDate,modStartDate,endDate;
    string symbolName;

    PriceTable* table;
    CashFlow flow;
    OrderStats stats;

    chrono::year_month_day curDate;

    double curPrice = 0;
    double curBal = 0;
    double curTR = 0;
    double DMplus = 0,DMminus = 0;
    double ATR = 0;
    double alphaATR = 0;
    double DIplus = 0,DIminus = 0;
    double DX = 0;
    double ADX = 0;
    ADXStrat(int n,int x,double adx_threshold,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName);
    void buy();
    void sell(); 
    void first(int startDateLoc);
    void writeCashFlow(chrono::year_month_day curDate);
    void writeFinalPNL();
    void check();
    void squareOff();
    void writeCSVfiles();
    double max(double a,double b);
    double max(double a,double b,double c);
    void main();
    void multiMain(PriceTable* srcTable);
};