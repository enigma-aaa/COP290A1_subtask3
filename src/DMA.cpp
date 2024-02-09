#include "parser.h"
#include "dateUtil.h"
#include <cmath>
class DMA
{
public:
    int n;
    int x;
    double p;

    chrono::year_month_day startDate, endDate, modStartDate;
    string symbolName;

    PriceTable table;
    CashFlow flow;
    OrderStats stats;

    double curPrice;
    double cursquaredSum;
    double curSum ;
    double curVar ;
    int noShares;
    double curBal;
    double curPrice ;
    int curLoc ;
    DMA(int n, int x, double p, chrono::year_month_day startDate, chrono::year_month_day endDate, string symbolName)
    {
        n = n;
        x = x;
        p = p;
        startDate = startDate;
        endDate = endDate;
        symbolName = symbolName;
        modStartDate = subtractDate(startDate, 2 * n);
    }

    void Buy(chrono :: year_month_day date)
    {
        noShares++ ;
        curBal = curBal - curPrice ;
        stats.addRow(date,"BUY",noShares,curPrice) ;
        flow.addRow(date,curBal) ;
    }
    void sell(chrono :: year_month_day date)
    {
        noShares-- ;
        curBal = curBal + curPrice ;
        stats.addRow(date,"SELL" ,noShares , curPrice) ;
        flow.addRow(date,curBal) ;
    }
    void check()
    {
        double sd = sqrt(cursquaredSum/n - ((curSum * curSum)/(n*n))) ;
        double curMean = curSum/n ;

        if(curPrice >= curMean + p*sd)
        {
            //buy
            if(noShares<x)
            {
                Buy(table.rows[curLoc].date) ;
            }
        }
        else if(curPrice < curMean-p*sd)
        {
            //sell
            if(noShares > -1*x )
            {
                sell(table.rows[curLoc].date) ;
            }
            
        }
        curSum = curSum -table.rows[curLoc-n].close + curPrice ;
        cursquaredSum = cursquaredSum - (table.rows[curLoc-n].close)*(table.rows[curLoc-n].close) + curPrice * curPrice ;
    }

    void squareOff()
    {
        if(noShares > 0)
        {
            curBal  = curBal + noShares * table.rows[table.rows.size()-1].close ; 
            stats.addRow(table.rows.back().date , "SELL" , noShares , table.rows.back().close) ;
            noShares = 0 ;
        }
    
    }

    void main()
    {
        table = getPriceTable(symbolName, modStartDate, endDate);
        int tableSize = table.rows.size();
        int startDateLoc = -1;
        curPrice = 0 ;
        for (int i = 0; i < table.rows.size(); i++)
        {
            if (table.rows[i].date == startDate)
            {
                startDateLoc = i;
            }
        }
        if (startDateLoc == -1)
        {
            cout << "start date not located in the table for some reason" << endl;
        }
        int startDate_n_Loc = startDateLoc - n;
        curSum = 0 ;
        for(int i = startDate_n_Loc ; i<startDate_n_Loc+n ; i++)
        {
            curSum +=  table.rows[i].close ;
        }
        cursquaredSum = 0 ;
        for(int i = startDate_n_Loc ; i<startDate_n_Loc+n ; i++)
        {
            cursquaredSum += table.rows[i].close * table.rows[i].close ;
        }    


        for(int i = startDateLoc ; i<  table.rows.size()-1 ; i++)
        {
            curPrice = table.rows[i].close ;
            curLoc = startDateLoc ;
            check() ;
        }
        squareOff() ;
    }
};