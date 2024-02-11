#pragma once 
#include "parser.h"
#include "dateUtil.h"
#include <cmath>
#include <iomanip>
class Pairs{
public:
    int x;
    int n;
    int noShares = 0;
    double threshold;

    string symbol1,symbol2;
    chrono::year_month_day startDate,endDate;
    chrono::year_month_day modStartDate;

    PriceTable* table1;
    PriceTable* table2;
    CashFlow flow;
    OrderStats stats1;
    OrderStats stats2;

    chrono::year_month_day curDate;

    Pairs(int x,int n,double threshold,
    chrono::year_month_day startDate,
    chrono::year_month_day endDate,string symbol1,
    string symbol2):x(x),n(n),threshold(threshold),startDate(startDate),
    endDate(endDate),symbol1(symbol1),symbol2(symbol2){
        modStartDate = subtractDate(startDate,2*n);
        cout << "called pairs constructor" << endl;
    }
    double curPrice1 = 0;
    double curPrice2 = 0;
    double curSpread = 0;
    double curSum = 0;
    double curSqSum = 0;
    double curBal = 0;

    void buy(){
        noShares++;
        curBal = curBal - curPrice1 + curPrice2;
        stats1.addRow(curDate,"BUY",1,curPrice1);
        stats2.addRow(curDate,"SELL",1,curPrice2);
    }
    void sell(){
        noShares--;
        curBal = curBal + curPrice1 - curPrice2;
        stats1.addRow(curDate,"SELL",1,curPrice1);
        stats2.addRow(curDate,"BUY",1,curPrice2);
    }
    void first(int startDateLoc){        
        int startDate_n_Loc = startDateLoc - n;
        for(int i=startDate_n_Loc;i<startDateLoc;i++){
            curPrice1 = table1->rows[i].close;
            curPrice2 = table2->rows[i].close;
            curSum += (curPrice1 - curPrice2);                
            curSqSum += (curPrice1-curPrice2)*(curPrice1-curPrice2);
        }
    }
    void writeCashFlow(chrono::year_month_day curDate){
        flow.addRow(curDate,curBal);
    }
    void writeFinalPNL(){
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
    void check(){
        double curMean = curSum/n;
        double sqDev = curSqSum/n - curMean*curMean;    
        double zScore = (curSpread - curMean)/sqDev;
        cout << "zScore is:" << zScore << endl;
        if(zScore > threshold){
            if(noShares > -x){
                sell();
            }
        }
        if(zScore < -threshold){
            if(noShares < x){
                buy();
            }
        }
    }
    void squareOff(){
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
    void writeCSVfiles()
    {
        string baseFilePath = "./bin/stockCSV/";
        string csv_cashflow = baseFilePath + "cashflow.csv";
        string csv_order_stats1 = baseFilePath + "order_stats1.csv";
        string csv_order_stats2 = baseFilePath + "order_stats2.csv";
        flow.writeToCsv(csv_cashflow);
        stats1.writeToCsv(csv_order_stats1);
        stats2.writeToCsv(csv_order_stats2);        
    }
    void main(){
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
};