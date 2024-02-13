#include "Pairs.h"
Pairs::Pairs(int x,int n,double threshold,
chrono::year_month_day startDate,
chrono::year_month_day endDate,string symbol1,
string symbol2):x(x),n(n),threshold(threshold),startDate(startDate),
endDate(endDate),symbol1(symbol1),symbol2(symbol2){
    modStartDate = subtractDate(startDate,max(2*n,30));
    cout << "called pairs constructor" << endl;
    cout<<"threshold is"<< threshold <<endl;
}

void Pairs::buy(){
    noShares++;
    curBal = curBal - curPrice1 + curPrice2;
    stats1.addRow(curDate,"BUY",1,curPrice1);
    stats2.addRow(curDate,"SELL",1,curPrice2);
}
void Pairs::sell(){
    noShares--;
    curBal = curBal + curPrice1 - curPrice2;
    stats1.addRow(curDate,"SELL",1,curPrice1);
    stats2.addRow(curDate,"BUY",1,curPrice2);
}
void Pairs::first(int startDateLoc){        
    int startDate_n_Loc = startDateLoc - n;
    for(int i=startDate_n_Loc;i<startDateLoc;i++){
        curPrice1 = table1->rows[i].close;
        curPrice2 = table2->rows[i].close;
        curSum += (curPrice1 - curPrice2);                
        curSqSum += (curPrice1-curPrice2)*(curPrice1-curPrice2);
    }
}
void Pairs::writeCashFlow(chrono::year_month_day curDate){
    flow.addRow(curDate,curBal);
}
void Pairs::writeFinalPNL(){
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
void Pairs::check(){
    curMean = curSum/n;
    curDev = curSqSum/n - curMean*curMean;   
    curDev = sqrt(curDev); 
    curZscore = (curSpread - curMean)/curDev;
    cout << "zScore is:" << curZscore <<" "<< threshold << endl;
    if(curZscore > threshold){
        if(noShares > -x){
            sell();
        }
    }
    if(curZscore < -threshold){
        if(noShares < x){
            buy();
        }
    }
}
void Pairs::squareOff(){
    curBal = curBal + noShares*(curPrice1-curPrice2);   
}
void Pairs::writeCSVfiles()
{
    string baseFilePath = "./bin/stockCSV/";
    string csv_cashflow = baseFilePath + "cashflow.csv";
    string csv_order_stats1 = baseFilePath + "order_stats1.csv";
    string csv_order_stats2 = baseFilePath + "order_stats2.csv";
    flow.writeToCsv(csv_cashflow);
    stats1.writeToCsv(csv_order_stats1);
    stats2.writeToCsv(csv_order_stats2);        
}
void Pairs::main(){
    PriceTable createTable1 = getPriceTable(symbol1,modStartDate,endDate);        
    PriceTable createTable2 = getPriceTable(symbol2,modStartDate,endDate);
    table1 = &createTable1;
    table2 = &createTable2;
    curPrice1 = 0;
    curPrice2 = 0;
    int startDateLoc = -1;
    for(int i=0;i<table1->rows.size();i++){
        if(grtrEqual(table1->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }
    if(startDateLoc == -1){
        cout << "start date not located in table for some reason" << endl;
    }
    first(startDateLoc);
    for(int i=startDateLoc;i<table1->rows.size();i++){
        curPrice1 = table1->rows[i].close;
        curPrice2 = table2->rows[i].close;
        //assuming current rows same in both
        curSpread = curPrice1 - curPrice2;
        curDate = table1->rows[i].date;
        double oldPrice1 = table1->rows[i-n].close;
        double oldPrice2 = table2->rows[i-n].close;
        curSum = curSum - (oldPrice1 - oldPrice2) + (curPrice1 - curPrice2);
        curSqSum = curSqSum - (oldPrice1 - oldPrice2)*(oldPrice1-oldPrice2) + (curPrice1 - curPrice2)*(curPrice1-curPrice2);
        check();
        writeCashFlow(curDate);
    }
    writeCSVfiles();
    squareOff();
    writeFinalPNL();
}