#include "parser.h"
#include "dateUtil.h"
#include <cmath>
class MACD
{
    int x;
    int n1 = 12;
    int n2 = 26;
    int nSign = 9;
    double alpha = 2.0/13.0;
    double shortEWM,longEWM;
    double signal;

    chrono::year_month_day startDate,endDate,modStartDate;    
    string symbolName;

    PriceTable table;
    CashFlow flow;
    OrderStats stats;

    double curPrice;
    chrono::year_month_day curDate;
    double curBal;
    int noShares;
    MACD(int x):x(x){
        noShares = 0;
        curBal = 0;
        modStartDate = subtractDate(startDate,2*n2);
    }
    void buy(){
        noShares++;
        curBal = curBal - curPrice;
        stats.addRow(curDate,"BUY",noShares,curPrice);
        flow.addRow(curDate,curBal);
    }
    void sell(){
        noShares--;
        curBal = curBal + curPrice;
        stats.addRow(curDate,"SELL",noShares,curPrice);
        flow.addRow(curDate,curBal);
    }
    void check(){
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
    void squareOff(){
        if(noShares > 0){
            curBal = curBal + noShares * curPrice;
            stats.addRow(table.rows.back().date,"SELL",noShares,curPrice);
            noShares = 0;
            flow.addRow(date,curBal);
        }
        if(noShares < 0){
            curBal = curBal + noShares * curPrice;
            stats.addRow(table.rows.back().date,"BUY",noShares,curPrice);
            noShares = 0;
            flow.addRow(date,curBal);
        }
    }
    void first(){
        int startDate_n1_Loc = startDate - n1;
        int startDate_n2_Loc = startDate - n2;
        int startDate_nSign_Loc = startDate - nSign;
        /* confirm that intially has to zero*/
        longEWM = 0;
        shortEWM = 0;
        signal = 0;
        /* have to recheck all the limits here may 
            require less than equal to instead of less than in many places*/
        for(int i=startDate_n1_Loc;i<startDate;i++){
            longEWM = alpha*(table.rows[i].close - longEWM) + longEWM;
        }
        for(int i=startDate_n2_Loc;i<startDate;i++){
            shortEWM = alpha*(table.rows[i].close - shortEWM) + shortEWM; 
        }
        /* EWM of MACD over 9 days required not directly 
            have to change this code*/
        for(int i=startDate_nSign_Loc;i<startDate;i++){
            signal = alpha*(table.rows[i].close - signal) + signal;
        }
    }
    void main(){
        table = getPriceTable(symbolName,modStartDate,endDate);
        curPrice = 0;
        for(int i=0;i<table.rows.size();i++){
            if(table.rows[i].date == startDate){
                startDateLoc = i;
            }
        }
        if(startDateLoc == -1){
            cout << "start date not located in table for some reason" << endl;
        }
        first();
        //have to check start indices here also
        for(int i = startDateLoc; i < table.rows.size();i++)
        {
            curPrice = table.rows[i].close;
            curDate = table.rows[i].date;
            longEWM = alpha*(curPrice - longEWM) + longEWM;
            shortEWM = alpha*(curPrice - shortEWM) + shortEWM;
            MACD = shortEWM - longEWM;
            signal = alpha*(MACD - signal) + signal;
            check();
        }
        squareOff();
    }
};