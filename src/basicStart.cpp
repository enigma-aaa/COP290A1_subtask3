#include "basicStart.h"
#include "CommonCons.h"

Basic::Basic(int n,int x,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName):
n(n),x(x),startDate(startDate),endDate(endDate),symbolName(symbolName){
    modStartDate = subtractDate(startDate,max(2*n,30));
    table = nullptr;
}
Basic::Basic(){}
/*first price opver here is not the price at the start 
    date but price n days before the start date*/
void Basic::firstPrice(double orgPrice){
    curPrice = orgPrice;
    noIncDays = 0;
    noDecDays = 0;
    curBal = 0;
    noShares = 0;
}
void Basic::nextPrice(double newPrice,chrono::year_month_day newDate){
    double oldPrice = curPrice;
    curPrice = newPrice;
    //cout << "oldPrice:" << oldPrice << " newPrice:" << newPrice << endl;
    if(oldPrice < newPrice){
        noIncDays++;
        noDecDays = 0;
        if(noIncDays == n){
            if(noShares < x){  buy(newDate); }
            noIncDays--;
        }
    }
    if(newPrice < oldPrice){
        noDecDays++;
        noIncDays = 0;
        if(noDecDays == n){
            if(noShares > -x){sell(newDate);}
            noDecDays--;
        }
    }
    if(newPrice == oldPrice){
        noIncDays = 0;
        noDecDays = 0;            
    }
}    
void Basic::buy(chrono::year_month_day date){  
    noShares++;
    curBal = curBal - curPrice;
    //Assuming showing new quantity here
    //cout << "Trying to addRow in buy" << endl;
    //quantity here is 1
    if(curLoc >= globStartLoc){
        stats.addRow(date,"BUY",1,curPrice);
    }
    //cout << "Added buy row" << endl;
}
void Basic::sell(chrono::year_month_day date){
    noShares--;
    curBal = curBal + curPrice;
    //cout << "Trying to addRow in sell" << endl;
    //quanity here is 1
    if(curLoc >= globStartLoc){
        stats.addRow(date,"SELL",1,curPrice);
    }
    //cout << "Added sell row" << endl;
}
/*Ensure price set to closing price before calling*/
void Basic::squareOff(chrono::year_month_day date){
    curBal = curBal + noShares*curPrice;
    noShares = 0;
    //do we need to write this to cashflow or to stats
}
void Basic::writeCashFlow(chrono::year_month_day date){
    flow.addRow(date,curBal);
}
void Basic::writeCSVfiles(){
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats = baseFilePath + ORDER_STATS;
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);
}
void Basic::writeFinalPNL(){
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
void Basic::main(){
    PriceTable createTable = getPriceTable(symbolName,modStartDate,endDate);
    multiMain(&createTable);
    writeCSVfiles();
    writeFinalPNL();
}
void Basic::multiMain(PriceTable* srcTable){
    table = srcTable;
    int tableSize = table->rows.size();
    int startDateLoc = table->rows.size();
    for(int i=0;i<table->rows.size();i++){
        if(grtrEqual(table->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }
    if(startDateLoc == -1) { cout << "start date not located in the table for some reason" << endl;}
    //cout << "startDateLoc is:" << startDateLoc << endl;
    globStartLoc = startDateLoc;
    int startDate_n_Loc = startDateLoc - n;
    //cout << "startDaten_Loc is:" << startDate_n_Loc << endl;
    double firstprice = table->rows[startDate_n_Loc].close;
    firstPrice(firstprice);


    for(int i=startDate_n_Loc+1;i<table->rows.size();i++){
        double newPrice = table->rows[i].close;
        curLoc = i;
        //cout << "Read newPrice" << endl;
        chrono::year_month_day curDate = table->rows[i].date;
        //cout << "read newDate" << endl;
        nextPrice(newPrice,curDate);
        //cout << "executed nexPrice" << endl;
        if(i >= startDateLoc){
            writeCashFlow(curDate);
        }
        //cout << "exectuted writeCashFlow" << endl;
        //cout << "currently noInc is:" << noIncDays << endl;
        //cout << "currently noDec is:" << noDecDays << endl;
        //cout << "Wrote cashflow for i=" << i << endl;
    }
    //writeCSVfiles();
    squareOff(table->rows.back().date);
    //writeFinalPNL();
}