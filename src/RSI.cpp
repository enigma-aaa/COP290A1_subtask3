#include "RSI.h"
#include "CommonCons.h"

RSI::RSI(int n,int x,double oversold_threshold,double overbought_threshold,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName)
:n(n),x(x),oversold_threshold(oversold_threshold),overbought_threshold(overbought_threshold),startDate(startDate),endDate(endDate),symbolName(symbolName){
    modStartDate = subtractDate(startDate,max(2*n,30));
    table = nullptr;
}
RSI::RSI() {}
void RSI::buy()
{
    noShares++ ;
    curBal = curBal - curPrice ;
    stats.addRow(curDate,"BUY",1,curPrice) ;
}
void RSI::sell()
{
    noShares-- ;
    curBal = curBal + curPrice ;
    stats.addRow(curDate,"SELL" ,1 , curPrice) ;
}    
void RSI::writeCashFlow(){
    flow.addRow(curDate,curBal);
}
void RSI::firstPrice(int startDateLoc){
    int startDate_n_Loc = startDateLoc - n;
    curBal = 0;
    noShares = 0;
    curGainSum = 0;
    curLossSum = 0;
    for(int i=startDate_n_Loc;i<startDateLoc;i++){
        double curGain = max(table->rows[i].close   - table->rows[i-1].close,0.0);
        double curLoss = max(table->rows[i-1].close - table->rows[i].close  ,0.0);
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
        if(noShares > -x){
            sell();
        }
    }
}
void RSI::writeCSVfiles(){
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats = baseFilePath + ORDER_STATS;
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);  
}
void RSI::writeFinalPNL(){
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
void RSI::main()
{
    PriceTable createTable = getPriceTable(symbolName,modStartDate,endDate);
    multiMain(&createTable);
    writeCSVfiles();
    writeFinalPNL();
}
void RSI::squareOff(){
    curBal = curBal + noShares*curPrice;
    noShares = 0;
}
void RSI::multiMain(PriceTable* srcTable)
{
    table = srcTable;
    int tableSize = table->rows.size();
    int startDateLoc = table->rows.size();
    for(int i=0;i<table->rows.size();i++){
        if(grtrEqual(table->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }

    firstPrice(startDateLoc);
    for(int i=startDateLoc;i<table->rows.size();i++){
        curDate = table->rows[i].date;
        curPrice = table->rows[i].close;
        //cout << "curDate:" << int(curDate.year()) << "/" << unsigned(curDate.month()) << "/" << unsigned(curDate.day()) << " ";
        double oldProfit = max(table->rows[i-n].close   - table->rows[i-n-1].close,0.0);
        double curProfit = max(table->rows[i].close     - table->rows[i-1].close,0.0);

        double oldLoss   = max(table->rows[i-n-1].close - table->rows[i-n].close,0.0);
        double curLoss   = max(table->rows[i-1].close   - table->rows[i].close,0.0);
        curGainSum = curGainSum - oldProfit + curProfit;
        curLossSum = curLossSum - oldLoss + curLoss;
        //cout << " curGainSum:" << curGainSum/n << " curLossSum:" << curLossSum/n;
        check();
        writeCashFlow();            

    }        
    //writeCSVfiles();
    squareOff();
    //writeFinalPNL();
}