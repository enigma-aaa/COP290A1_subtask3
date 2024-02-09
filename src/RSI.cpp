#include "parser.h"
#include "dateUtil.h"
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

    PriceTable table;
    CashFlow flow;
    OrderStats stats;

    double curPrice;
    double curGainSum;
    double curLossSum;
    double curBal;

    DMA(int n,int x,double oversold_threshold,double overbought_threshold,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName)
    :n(n),x(x),p(p),startDate(startDate),endDate(endDate),symbolName(symbolName){
        modStartDate = subtractDate(startDate,2*n);
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
    void firstPrice(int startDateLoc){
        int startDate_n_Loc = startDateLoc - n;
        curBal = 0;
        noShares = 0;
        for(int i=startDate_n_Loc;i<startDateLoc;i++){
            double curGain = max(table.rows[i].price - table.rows[i-1].price,0);
            double curLoss = max(table.rows[i-1].price - table.rows[i].price,0);
            curGainSum += curGain;
            curGainLoss += curLoss;
        }
    }
    void check()
    {
        double RS = curGainSum/curLossSum;
        double RSI = 100 - 100/(1+RS);
        if(RSI < oversold_threshold){
            if(noShares < x){
                buy();
            }
        }
        if(RSI > overbought_threshold){
            if(noShares > x){
                sell();
            }
        }
    }
    void writeCashFlow(){
        flow.addRow(date,curBal);
    }
    void writeCSVfiles(){
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
        table = getPriceTable(symbolName,modStartDate,endDate);
        int tableSize = table.rows.size();
        int startDateLoc = -1;
        for(int i=0;i<table.rows.size();i++){
            if(table.rows[i].date == startDate){
                startDateLoc = i;
            }
        }
        if(startDateLoc == -1){ cout << "start date not located in the table" << endl;}

        firstPrice(startDateLoc);
        for(int i=startDateLoc;i<table.rows.size();i++){
            double oldProfit = max(table.rows[i-n]-table.rows[i-n-1],0);
            double oldLoss   = max(table.rows[i-n-1]-table.rows[i-n],0);
            double curProfit = max(table.rows[i]-table.rows[i-1],0);
            double curLoss   = max(table.rows[i-1]-table.rows[i],0);
            curGainSum = curGainSum - oldProfit + curProfit;
            curLossSum = curGainSum - oldLoss + curLoss;
            check();
            writeCashFlow();            
        }        
        writeCSVfiles();
        squareOff();
        writeFinalPNL();
    }
    void squareOff(){
        curBal = curBal + noShares*curPrice;
        noShares = 0;
    }
};