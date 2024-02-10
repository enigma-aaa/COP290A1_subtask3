#include "MACD.h"
MACDStrat::MACDStrat(int x,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName)
:x(x),startDate(startDate),endDate(endDate),symbolName(symbolName){
    noShares = 0;
    curBal = 0;
    modStartDate = subtractDate(startDate,2*n2);
    cout << "modStartDate is: year:" << int(modStartDate.year()) << " month:" << unsigned(modStartDate.month()) << " day:" << unsigned(modStartDate.day()) << endl;
    table = nullptr;
}
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
        stats.addRow(table->rows.back().date,"SELL",noShares,curPrice);
        noShares = 0;
    }
    if(noShares < 0){
        curBal = curBal + noShares * curPrice;
        stats.addRow(table->rows.back().date,"BUY",noShares,curPrice);
        noShares = 0;
    }
}
void MACDStrat::first(int startDateLoc){
    /*
    int startDate_n1_Loc = startDateLoc - n1 + 1;
    int startDate_n2_Loc = startDateLoc - n2 + 1;
    int startDate_nSign_Loc = startDateLoc - nSig + 1;
    //confirm that intially has to zero
    longEWM = 0;
    shortEWM = 0;
    //have to recheck all the limits here may 
    //require less than equal to instead of less than in many places
    //again here also when including curDate do we take only n1-1 elements definetly confirm with sir
    for(int i=startDate_n1_Loc;i<startDate_n1_Loc +n1;i++){
        longEWM = alpha1*(table->rows[i].close - longEWM) + longEWM;
    }
    for(int i=startDate_n2_Loc;i<startDate_n2_Loc + n2;i++){
        shortEWM = alpha2*(table->rows[i].close - shortEWM) + shortEWM; 
    }
    //Signal = 0 at time t= 0
    signal = 0;
    check();
    */
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
    table = &curTable;
    curPrice = 0;
    int startDateLoc = -1;
    for(int i=0;i<table->rows.size();i++){
        if(table->rows[i].date == startDate){
            startDateLoc = i;
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
        longEWM = alpha1*(curPrice - longEWM) + longEWM;
        shortEWM = alpha2*(curPrice - shortEWM) + shortEWM;
        MACD = shortEWM - longEWM;
        signal = alphaSig*(MACD - signal) + signal;
        check();
        writeCashFlow(table->rows[i].date);
    }
    writeCSVfiles();
    squareOff();
    writeFinalPNL();
}
void MACDStrat::multiMain(PriceTable* srcTable){
    table = srcTable;
    curPrice = 0;
    int startDateLoc = -1;
    for(int i=0;i<table->rows.size();i++){
        if(table->rows[i].date == startDate){
            startDateLoc = i;
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
        longEWM = alpha1*(curPrice - longEWM) + longEWM;
        shortEWM = alpha2*(curPrice - shortEWM) + shortEWM;
        MACD = shortEWM - longEWM;
        signal = alphaSig*(MACD - signal) + signal;
        check();
        writeCashFlow(table->rows[i].date);
    }
    //writeCSVfiles();
    squareOff();
    //writeFinalPNL();
}