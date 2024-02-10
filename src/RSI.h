#include "parser.h"
#include "dateUtil.h"
#include <iomanip>
#include <cmath>
class RSI{
public:
    int n;
    int x;
    int noShares;
    double oversold_threshold;
    double overbought_threshold;

    chrono::year_month_day startDate,endDate,modStartDate;
    string symbolName;

    chrono::year_month_day curDate;

    PriceTable table;
    CashFlow flow;
    OrderStats stats;

    double curPrice;
    double curGainSum;
    double curLossSum;
    double curBal;

    RSI(int n,int x,double oversold_threshold,double overbought_threshold,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName);
    void buy();
    void sell();   
    void writeCashFlow();
    void firstPrice(int startDateLoc);
    void check();
    void writeCSVfiles();
    void writeFinalPNL();
    void main();
    void squareOff();
};