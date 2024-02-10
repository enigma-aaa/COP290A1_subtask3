#include "parser.h"
#include "dateUtil.h"
#include <cmath>
#include <iomanip>
class MACDStrat
{
public:
    int x;
    int n1 = 12;
    int n2 = 26;
    int nSig = 9;
    double alpha1 = 2.0/(n1+1);
    double alpha2 = 2.0/(n2+1);
    double alphaSig = 2.0/(nSig+1);
    double MACD = 0;
    double shortEWM,longEWM;
    double signal;

    chrono::year_month_day startDate,endDate,modStartDate;    
    string symbolName;

    PriceTable table;
    CashFlow flow;
    OrderStats stats;

    double curPrice;
    chrono::year_month_day curDate;
    double curBal;
    int noShares;
    MACDStrat(int x,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName);
    void buy();
    void sell();
    void check();
    void squareOff();
    void first(int startDateLoc);
    void writeCashFlow(chrono::year_month_day curDate);
    void writeCSVfiles();
    void writeFinalPNL();
    void main();
};