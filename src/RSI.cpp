#include "RSI.h"

RSI::RSI(int n,int x,double oversold_threshold,double overbought_threshold,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName)
:n(n),x(x),startDate(startDate),endDate(endDate),symbolName(symbolName){
    modStartDate = subtractDate(startDate,2*n);
}
void RSI::buy()
{
    noShares++ ;
    curBal = curBal - curPrice ;
    stats.addRow(curDate,"BUY",noShares,curPrice) ;
}
void RSI::sell()
{
    noShares-- ;
    curBal = curBal + curPrice ;
    stats.addRow(curDate,"SELL" ,noShares , curPrice) ;
}    
void RSI::writeCashFlow(){
    flow.addRow(curDate,curBal);
}
void RSI::firstPrice(int startDateLoc){
    int startDate_n_Loc = startDateLoc - n;
    curBal = 0;
    noShares = 0;
    for(int i=startDate_n_Loc;i<startDateLoc;i++){
        double curGain = max(table.rows[i].close - table.rows[i-1].close,0.0);
        double curLoss = max(table.rows[i-1].close - table.rows[i].close,0.0);
        curGainSum += curGain;
        curLossSum += curLoss;
    }
}
void RSI::check()
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
void RSI::writeCSVfiles(){
    string baseFilePath = "./bin/stockCSV/";
    string csv_cashflow = baseFilePath + "cashflow.csv";
    string csv_order_stats = baseFilePath + "order_stats.csv";
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);  
}
void RSI::writeFinalPNL(){
    stringstream stream;
    stream << std::fixed << std::setprecision(2) << curBal;
    string curBalStr = stream.str();
    string baseFilePath = "./bin/stockCSV/";
    string pnlFileName = "finalPNL.txt";
    string pnlFilePath = baseFilePath + pnlFileName;
    ofstream pnlFile(pnlFilePath);
    pnlFile << curBalStr;
    pnlFile.close();
}
void RSI::main()
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
        curDate = table.rows[i].date;
        double oldProfit = max(table.rows[i-n].close-table.rows[i-n-1].close,0.0);
        double oldLoss   = max(table.rows[i-n-1].close-table.rows[i-n].close,0.0);
        double curProfit = max(table.rows[i].close-table.rows[i-1].close,0.0);
        double curLoss   = max(table.rows[i-1].close-table.rows[i].close,0.0);
        curGainSum = curGainSum - oldProfit + curProfit;
        curLossSum = curGainSum - oldLoss + curLoss;
        check();
        writeCashFlow();            
    }        
    writeCSVfiles();
    squareOff();
    writeFinalPNL();
}
void RSI::squareOff(){
    curBal = curBal + noShares*curPrice;
    noShares = 0;
}