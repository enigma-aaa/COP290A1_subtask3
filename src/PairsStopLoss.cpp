#include "PairsStopLoss.h"
#include <queue>
PairsStopLoss::PairsStopLoss(int x,int n,double threshold,double stop_loss_threshold,
chrono::year_month_day startDate,
chrono::year_month_day endDate,string symbol1,
string symbol2):x(x),n(n),threshold(threshold),startDate(startDate),
endDate(endDate),symbol1(symbol1),symbol2(symbol2){
    modStartDate = subtractDate(startDate,2*n);
    cout << "called pairs constructor" << endl;
}

void PairsStopLoss::buy(){
    noShares++;
    curBal = curBal - curPrice1 + curPrice2;
    if(!sellMean.empty()){
        sellMean.pop(); 
        sellStandDev.pop();
    }else{
        buyMean.push(curMean);
        buyStandDev.push(curDev);
    }
    stats1.addRow(curDate,"BUY",1,curPrice1);
    stats2.addRow(curDate,"SELL",1,curPrice2);
}

void PairsStopLoss::sell(){
    noShares--;
    curBal = curBal + curPrice1 - curPrice2;
    if(!buyMean.empty()){
        buyMean.pop();
        buyStandDev.pop();
    }else{
        sellMean.push(curMean);
        sellStandDev.push(curDev);
    }
    stats1.addRow(curDate,"SELL",1,curPrice1);
    stats2.addRow(curDate,"BUY",1,curPrice2);
}

void PairsStopLoss::first(int startDateLoc){        
    int startDate_n_Loc = startDateLoc - n;
    for(int i=startDate_n_Loc;i<startDateLoc;i++){
        curPrice1 = table1->rows[i].close;
        curPrice2 = table2->rows[i].close;
        curSum += (curPrice1 - curPrice2);                
        curSqSum += (curPrice1-curPrice2)*(curPrice1-curPrice2);
    }
}

void PairsStopLoss::writeCashFlow(chrono::year_month_day curDate){
    flow.addRow(curDate,curBal);
}
void PairsStopLoss::writeFinalPNL(){
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
void PairsStopLoss::check(){
    curMean = curSum/n;
    curDev = curSqSum/n - curMean*curMean;   
    curDev = sqrt(curDev); 
    curZscore = (curSpread - curMean)/curDev;
    cout << "zScore is:" << curZscore << endl;
    if(curZscore > threshold){
        if(noShares > -x){
            sell();
            minHeap.push(curDev*stop_loss_threshold + curMean) ;
            maxHeap.push(curDev*(-1) * stop_loss_threshold + curMean) ;
        }
    }
    if(curZscore < -threshold){
        if(noShares < x){
            buy();
            minHeap.push(curDev*stop_loss_threshold + curMean) ;
            maxHeap.push(curDev*(-1) * stop_loss_threshold + curMean) ;
        }
    }    

    if(minHeap.empty() == false && curSpread >= minHeap.top())
    {
        while(minHeap.empty() == false && curSpread >= minHeap.top())
        {   
            minHeap.pop() ;
            buy() ;
        }
    }
    else if(maxHeap.empty() == false && curSpread <= maxHeap.top())
    {
        while(maxHeap.empty() == false && curSpread <= maxHeap.top())
        {
            maxHeap.pop() ;
            sell() ;
        }
    }

}
void PairsStopLoss::squareOff(){
    if(noShares > 0){
        curBal = curBal + noShares*(curPrice1-curPrice2);
        stats1.addRow(table1->rows.back().date,"SELL",noShares,curPrice1);
        stats2.addRow(table2->rows.back().date,"BUY",noShares,curPrice2);            
    }   
    if(noShares < 0){
        curBal = curBal + noShares*(curPrice1-curPrice2);
        stats1.addRow(table1->rows.back().date,"BUY",-noShares,curPrice1);
        stats2.addRow(table2->rows.back().date,"SELL",-noShares,curPrice2);
    } 
}

void PairsStopLoss::writeCSVfiles()
{
    string baseFilePath = "./bin/stockCSV/";
    string csv_cashflow = baseFilePath + "cashflow.csv";
    string csv_order_stats1 = baseFilePath + "order_stats1.csv";
    string csv_order_stats2 = baseFilePath + "order_stats2.csv";
    flow.writeToCsv(csv_cashflow);
    stats1.writeToCsv(csv_order_stats1);
    stats2.writeToCsv(csv_order_stats2);        
}

void PairsStopLoss::main(){
    PriceTable createTable1 = getPriceTable(symbol1,modStartDate,endDate);        
    PriceTable createTable2 = getPriceTable(symbol2,modStartDate,endDate);
    table1 = &createTable1;
    table2 = &createTable2;
    curPrice1 = 0;
    curPrice2 = 0;
    int startDateLoc = -1;
    for(int i=0;i<table1->rows.size();i++){
        if(table1->rows[i].date == startDate){
            startDateLoc = i;
        }
    }
    if(startDateLoc == -1){
        cout << "start date not located in table for some reason" << endl;
    }
    first(startDateLoc);
    //for handle stop loss threshold instead of just queue might have to keep a linked list here
    for(int i=startDateLoc;i<table1->rows.size();i++){
        curPrice1 = table1->rows[i].close;
        curPrice2 = table2->rows[i].close;
        //assuming current rows same in both
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