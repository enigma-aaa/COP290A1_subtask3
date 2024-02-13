#include "MACD.h"
#include "CommonCons.h"
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
    stats.addRow(curDate,"BUY",1,curPrice);
}
void MACDStrat::sell(){
    noShares--;
    curBal = curBal + curPrice;
    stats.addRow(curDate,"SELL",1,curPrice);
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
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats = baseFilePath + ORDER_STATS;
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);        
}
void MACDStrat::writeFinalPNL(){
    stringstream stream;
    stream << std::fixed << std::setprecision(2) << curBal;
    string curBalStr = stream.str();
    string baseFilePath = BASE_FILE_PATH;
    string pnlFileName = FINAL_PNL;
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
    writeFinalPNL();
}
void MACDStrat::multiMain(PriceTable* srcTable){
    table = srcTable;
    curPrice = 0;
    int startDateLoc = table->rows.size();
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
    writeCashFlow(table->rows[startDateLoc].date);
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