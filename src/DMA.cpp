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
    //double curPrice ;
    int curLoc ;
    DMA(int n, int x, double p, chrono::year_month_day startDate, chrono::year_month_day endDate, string symbolName):
    n(n),x(x),p(p),startDate(startDate),endDate(endDate),symbolName(symbolName)
    {
        modStartDate = subtractDate(startDate, 2 * n);
    }

    void buy(chrono :: year_month_day date)
    {
        noShares++ ;
        curBal = curBal - curPrice ;
        stats.addRow(date,"BUY",noShares,curPrice) ;
    }
    void sell(chrono :: year_month_day date)
    {
        noShares-- ;
        curBal = curBal + curPrice ;
        stats.addRow(date,"SELL" ,noShares , curPrice) ;
    }
    void writeCashFlow(chrono::year_month_day curDate){
        flow.addRow(date,curBal);
    }
    void check()
    {
        double sd = sqrt(cursquaredSum/n - ((curSum * curSum)/(n*n))) ;
        double curMean = curSum/n ;

        /*Both buy and sell may occur here because of greater than or equal to*/
        if(curPrice >= curMean + p*sd)
        {
            //buy
            if(noShares<x)
            {
                buy(table.rows[curLoc].date) ;
            }
        }
        else if(curPrice <= curMean-p*sd)
        {
            //sell
            if(noShares > -1*x )
            {
                sell(table.rows[curLoc].date) ;
            }
            
        }
        /*Included -(n-1) instead of -n here becuase sir said we have to include current day and seemed to say total n days*/
        curSum = curSum -table.rows[curLoc-n+1].close + curPrice ;
        cursquaredSum = cursquaredSum - (table.rows[curLoc-n+1].close)*(table.rows[curLoc-n+1].close) + curPrice * curPrice ;
    }

    void squareOff()
    {
        curBal  = curBal + noShares * table.rows[table.rows.size()-1].close ; 
        noShares = 0 ;
    }
    void writeCSVfiles()
    {
        string baseFilePath = "./bin/stockCSV/"
        string csv_cashflow = baseFilePath + "cashflow.csv";
        string csv_order_stats = baseFilePath + "order_stats.csv";
        cashflow.writeToCsv(csv_cashflow);
        stats.writeToCsv(csv_order_stats);        
    }
    void writeFinalPNL(){
        stringstream stream;
        stream << std::fixed << std::setprecision(2) << curBal;
        string curBalStr = stream.str();
        string baseFilePath = "./bin/stockCSV/"
        string pnlFileName = "finalPNL.txt";
        string pnlFilePath = baseFilePath + pnlFileName;
        ofstream pnlFile(pnlFilePath);
        pnlFile << curBalStr;
        pnlFile.close();
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
        /*Have to ask sir since current day has to be included might have to do n-1 here*/
        int startDate_n_Loc = startDateLoc - (n-1);
        curSum = 0 ;
        cursquaredSum = 0;
        for(int i = startDate_n_Loc ; i<startDate_n_Loc+n ; i++)
        {
            curSum +=  table.rows[i].close ;
            cursquaredSum += table.rows[i].close * table.rows[i].close;
        }   


        for(int i = startDateLoc ; i<  table.rows.size()-1 ; i++)
        {
            curPrice = table.rows[i].close ;
            curLoc = i;
            check() ;
            writeCashFlow(table.rows[i].date);
        }
        writeCSVfiles();
        squareOff() ;
        writeFinalPNL();
    }
}