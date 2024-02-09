#include "parser.h"
#include "dateUtil.h"
#include <queue>
#include <cmath>
class DMAPlus
{
public:
    int n;
    int x;
    double p;
    int max_hold_days;
    double c1,c2;

    chrono::year_month_day startDate,endDate,modStartDate;
    string symbolName;

    /*only add trades which have not been balanced*/
    /*not both sellDates and buyDates can have elements 
        recheck to confirm*/
    queue<chrono::year_month_day> sellDates;
    queue<chrono::year_month_day> buyDates;

    PriceTable table;
    CashFlow flow;
    OrderStats stats;

    double curPrice;
    chrono::year_month_day curDate;
    double curAbsoluteSum;
    double priceChange_n;
    double smoothingFactor;
    double AMA;
    double ER;
    DMAPlus(int n,int x,double p,int max_hold_days,double c1,double c2)
    :n(n),x(x),p(p),max_hold_days(max_hold_days),c1(c1),c2(c2){}

    //first+n is the index of the start function
    void firstPrice(int first){
        curAbsoluteSum = 0;
        for(int i=first;i<n+first;i++){            
            double curDiff = abs(table.rows[i] - table.rows[i+1]);
            curAbsoluteSum += curDiff;    
        }
        //priceChange_n = table.rows[first+n] - table.rows[first];
        //not needed here handeled in the loop
        smoothingFactor = 0.5;
        AMA = table.rows[first+n];
        
    }
    void buy()
    {
        noShares++;
        curBal = curBal - curPrice;
        //forgot to add quantity column have to handle that
        stats.addRow(curDate,"BUY",noShares,curPrice);
        flow.addRow(curDate,curBal);
        if(!sellDates.empty()){
            //if have sold some stock as in shorted it but have not balanced it out then do this
            //sellDates.front();
            sellDates.pop();
        }else{
            buyDates.push(curDate);
        }
    }
    void sell()
    {
        noShares--;
        curBal = curBal + curPrice;
        stats.addRow(curDate,"SELL",noShares,curPrice);
        flow.addRow(curDate,curBal);
        if(!buyDates.empty()){
            buyDates.pop();
        }else{
            sellDates.push(curDate);
        }
    }
    void handleMaxHold(chrono::year_month_day curDate){
        if(!buyDates.empty()){
            chrono::year_month_day oldestBuy = buyDates.front();
            int dayDiff = difference(curDate,oldestBuy);
            //no need to check later elements in queue as only one of them can be oldest
            if(dayDiff > max_hold_days){
                noShares++;
                curBal = curBal - curPrice;
                stats.addRow(date,"SELL",noShares,curPrice);
                flow.addRow(date,curBal);
                buyDates.pop();
            }
        }
        if(!sellDates.empty()){
            chrono::year_month_day oldestSell = sellDates.front();
            int dayDiff = difference(curDate,oldestSell);
            if(dayDiff > max_hold_days){
                noShares--;
                curBal = curBal + curPrice;
                stats.addRow(date,"SELL",noShares,curPrice);
                flow.addRow(date,curBal);
                sellDates.pop();
            }
        }
    }
    void check(){
        double perCentDiff = ((AMA-curPrice)*100)/AMA;
        //cofirm p given here as pre multiplied as 100
        //assuming p is non zero
        if(perCentDiff >= p/100){
            buy();
        }
        if(perCentDiff <= -p/100){
            sell();
        }
    }
    void main(string symbolName){
        table = getPriceTable(symbolName,modStartDate,endDate);
        int tableSize = table.rows.size();
        int startDateLoc = -1;
        for(int i=0;i<table.rows.size();i++){
            if(table.rows[i].date == startDate){
                startDateLoc = i;
            }
        }
        if(startDateLoc == -1){ cout << "start date not located in the table" << endl;}

        int startDate_n_Loc = startDateLoc - n;
        firstPrice(startDate_n_Loc);
        //cross check starting index once
        for(int i=startDateLoc+1;i<table.rows.size();i++){
            priceChange_n = table.rows[i].close - table.rows[i-n].close;
            curDate = table.rows[i].date;
            //definetly have to crosscheck these indices
            double oldAbsDiff = abs(table.rows[i-n]-table.rows[i]);
            double newAbsDiff = abs(table.rows[i+1]-table.rows[i]);
            curAbsoluteSum = curAbsoluteSum - oldAbsDiff + newAbsDiff;
            ER = priceChange_n/curAbsoluteSum;
            double factorNum = 2*ER/(1+c2) - 1;
            double factorDenom = 2*ER/(1+c2) + 1;
            double factor = factorNum/factorDenom;
            smoothingFactor = smoothingFactor + c1 * (factor - smoothingFactor);
            AMA = AMA + smoothingFactor*(curPrice - AMA);
            check();
            handleMaxHold();
        }
        squareOff();
    }
    void squareOff(){
        if(noShares > 0){
            curBal = curBal + noShares * table.rows.back().close;
            stats.addRow(table.rows.back().date,"SELL",noShares,table.rows.back().close);
            flow.addRow(date,curBal);
            noShares = 0;
            //if correctly understood only have to clear this
            sellDates.clear();
        }
        if(noShares < 0){
            curBal = curBal + noShares * table.rows.back().close;
            stats.addRow(table.rows.back().date,"BUY",noShares,table.rows.back().close);
            flow.addRow(date,curBal);
            noShares = 0;
            buyDates.clear();
        }
    }
}