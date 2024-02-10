#include "ADX.h"
ADXStrat::ADXStrat(int n,int x,double adx_threshold,chrono::year_month_day startDate,chrono::year_month_day endDate):
n(n),x(x),adx_threshold(adx_threshold),startDate(startDate),endDate(endDate){
    noShares = 0;
    curBal = 0;
    modStartDate = subtractDate(startDate,2*n);
    alphaATR = 2.0/((double)(n+1));
}
void ADXStrat::buy(){
    noShares++;
    curBal = curBal - curPrice;
    stats.addRow(curDate,"BUY",noShares,curPrice);
}
void ADXStrat::sell(){
    noShares--;
    curBal = curBal + curPrice;
    stats.addRow(curDate,"SELL",noShares,curPrice);
} 
void ADXStrat::first(int startDateLoc){
    int startDate_n_Loc = startDateLoc -n;
    ATR = 0;
    for(int i = startDate_n_Loc;i<startDateLoc;i++){
        PriceTableRow& curRow = table.rows[i];
        PriceTableRow& prevRow = table.rows[i-1];
        curTR = max(max(curRow.high - curRow.low,curRow.high - curRow.prevClose),curRow.low - curRow.prevClose);
        DMplus = max(0.0,curRow.high - prevRow.high);
        DMminus = max(0.0,curRow.low - prevRow.low);
        ATR = alphaATR*(curTR - ATR) + ATR;
        double DMplusByATR = DMplus/ATR;
        double DMminusByATR = DMminus/ATR;
        DIplus = alphaATR*(DIplus - DMplusByATR) + DMplusByATR;
        DIminus = alphaATR*(DIminus - DMminusByATR) + DMminusByATR;
        DX = ((DIplus + DIminus)*100)/(DIplus - DIminus);
        ADX = alphaATR*(DX - ADX) + ADX;            
    }
    return;
}
void ADXStrat::writeCashFlow(chrono::year_month_day curDate){
    flow.addRow(curDate,curBal);
}
void ADXStrat::writeFinalPNL(){
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
void ADXStrat::check(){
    if(ADX > adx_threshold){
        buy();
    } 
    if(ADX < adx_threshold){
        sell();
    }
}
void ADXStrat::squareOff(){
    if(noShares > 0){
        curBal = curBal + noShares * curPrice;
        stats.addRow(table.rows.back().date,"SELL",noShares,curPrice);
        noShares = 0;
    }
    if(noShares < 0){
        curBal = curBal + noShares * curPrice;
        stats.addRow(table.rows.back().date,"BUY",noShares,curPrice);
        noShares = 0;
    }
}
void ADXStrat::writeCSVfiles()
{
    string baseFilePath = "./bin/stockCSV/";
    string csv_cashflow = baseFilePath + "cashflow.csv";
    string csv_order_stats = baseFilePath + "order_stats.csv";
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);        
}
double ADXStrat::max(double a,double b){
    if(a > b){
        return a;
    }
    return b;
}
void ADXStrat::main(){
    table = getPriceTable(symbolName,modStartDate,endDate);
    curPrice = 0;
    int startDateLoc = -1;
    for(int i=0;i<table.rows.size();i++){
        if(table.rows[i].date == startDate){
            startDateLoc = i;
        }
    }
    if(startDateLoc == -1){
        cout << "start date not located in table for some reason" << endl;
    }
    first(startDateLoc);
    for(int i=startDateLoc;i<table.rows.size();i++){
        curPrice = table.rows[i].close;
        curDate = table.rows[i].date;
        PriceTableRow& curRow = table.rows[i];
        PriceTableRow& prevRow = table.rows[i];
        curTR = max(max(curRow.high - curRow.low,curRow.high - curRow.prevClose),curRow.low - curRow.prevClose);
        DMplus = max(0.0,curRow.high - prevRow.high);
        DMminus = max(0.0,curRow.low - prevRow.low);
        ATR = alphaATR*(curTR - ATR) + ATR;
        double DMplusByATR = DMplus/ATR;
        double DMminusByATR = DMminus/ATR;
        DIplus = alphaATR*(DIplus - DMplusByATR) + DMplusByATR;
        DIminus = alphaATR*(DIminus - DMminusByATR) + DMminusByATR;
        DX = ((DIplus + DIminus)*100)/(DIplus - DIminus);
        ADX = alphaATR*(DX - ADX) + ADX;     
        check();
        writeCashFlow(curRow.date);
    }
    writeCSVfiles();
    squareOff();
    writeFinalPNL();
}