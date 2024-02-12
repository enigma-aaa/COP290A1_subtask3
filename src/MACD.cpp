#include "MACD.h"
MACDStrat::MACDStrat(int x,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName)
:x(x),startDate(startDate),endDate(endDate),symbolName(symbolName){
    noShares = 0;
    curBal = 0;
    modStartDate = subtractDate(startDate,2*n2);
    cout << "modStartDate is: year:" << int(modStartDate.year()) << " month:" << unsigned(modStartDate.month()) << " day:" << unsigned(modStartDate.day()) << endl;
    table = nullptr;
    dateFloat = DateFloat("Signal");
}
MACDStrat::MACDStrat() {}
void MACDStrat::buy(){
    noShares++;
    curBal = curBal - curPrice;
    stats.addRow(curDate,"BUY",noShares,curPrice);
}
void MACDStrat::sell(){
    noShares--;
    curBal = curBal + curPrice;
    stats.addRow(curDate,"SELL",noShares,curPrice);
}
void MACDStrat::check(){
    if(MACD > signal){
        if(noShares < x){
            buy();
        }            
    }
    if(MACD < signal){
        if(noShares > -x){
            sell();
        }
    }
}
void MACDStrat::squareOff(){
    if(noShares > 0){
        curBal = curBal + noShares * curPrice;
//        stats.addRow(table->rows.back().date,"SELL",noShares,curPrice);
        noShares = 0;
    }
    if(noShares < 0){
        curBal = curBal + noShares * curPrice;
//        stats.addRow(table->rows.back().date,"BUY",noShares,curPrice);
        noShares = 0;
    }
}
void MACDStrat::first(int startDateLoc){
    longEWM  = table->rows[startDateLoc].close;
    shortEWM = table->rows[startDateLoc].close;
    signal = 0;    
}
void MACDStrat::writeCashFlow(chrono::year_month_day curDate){
    flow.addRow(curDate,curBal);
}
void MACDStrat::writeCSVfiles()
{
    string baseFilePath = "./bin/stockCSV/";
    string csv_cashflow = baseFilePath + "cashflow.csv";
    string csv_order_stats = baseFilePath + "order_stats.csv";
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);        
}
void MACDStrat::writeDebugFiles()
{
    string baseFilePath = "./bin/stockCSV/";
    string debugFile = baseFilePath + "Signal.csv";
    dateFloat.writeToCsv(debugFile);
}
void MACDStrat::writeFinalPNL(){
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
void MACDStrat::main(){
    PriceTable curTable = getPriceTable(symbolName,modStartDate,endDate);
    multiMain(&curTable);
    squareOff();
    writeCSVfiles();
    writeDebugFiles();
    writeFinalPNL();
}
void MACDStrat::multiMain(PriceTable* srcTable){
    table = srcTable;
    curPrice = 0;
    int startDateLoc = -1;
    for(int i=0;i<table->rows.size();i++){
        if(grtrEqual(table->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }
    if(startDateLoc == -1){
        cout << "start date not located in table for some reason" << endl;
    }
    first(startDateLoc);
    //have to check start indices here also
    //handling startDateLoc already in check function have to start with startDateLoc+1
    for(int i = startDateLoc+1; i < table->rows.size();i++)
    {
        curPrice = table->rows[i].close;
        curDate = table->rows[i].date;
        longEWM = alphaLong*(curPrice - longEWM) + longEWM;
        shortEWM = alphaShort*(curPrice - shortEWM) + shortEWM;
        MACD = shortEWM - longEWM;
        signal = alphaSig*(MACD - signal) + signal;
        dateFloat.addRow(curDate,MACD -signal);
        check();
        //cout<<"signal "<<signal<<" macd "<<MACD<<" ";
        //printDate(curDate); cout << endl;
        writeCashFlow(table->rows[i].date);
    }
    //writeCSVfiles();
    squareOff();
    //writeFinalPNL();
}