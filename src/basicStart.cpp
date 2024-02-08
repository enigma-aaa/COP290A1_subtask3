#include "dateUtil.h"
#include <chrono>
#include "parser.h"
using namespace std;

class Basic{
    public:
        int n,x;
        chrono::year_month_day startDate,endDate,modStartDate;
        string symbolName;

        PriceTable table;   
        CashFlow flow;
        OrderStats stats;

        double curPrice;
        int noIncDays;
        int noDecDays;
        int noShares;
        double curBal;

    Basic(int n,int x,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName):
    n(n),x(x),startDate(startDate),endDate(endDate),symbolName(symbolName){
        modStartDate = subtractDate(startDate,2*n);
    }
    /*first price opver here is not the price at the start 
        date but price n days before the start date*/
    void firstPrice(double orgPrice){
        curPrice = orgPrice;
        noIncDays = 1;
        noDecDays = 1;
        curBal = 0;
        noShares = 0;
    }
    void nextPrice(double newPrice,chrono::year_month_day newDate){
        double oldPrice = curPrice;
        curPrice = newPrice;
        if(oldPrice < newPrice){
            noIncDays++;
            noDecDays = 1;
            if(noIncDays == n){
                if(noShares < x){  buy(newDate); }
                noIncDays--;
            }
        }
        if(newPrice < oldPrice){
            noDecDays++;
            noIncDays = 1;
            if(noDecDays == n){
                if(noShares > -x){sell(newDate);}
                noDecDays--;
            }
        }
        if(newPrice == curPrice){
            noIncDays = 1;
            noDecDays = 1;            
        }
    }    
    void buy(chrono::year_month_day date){  
        noShares++;
        curBal = curBal - curPrice;
        //Assuming showing new quantity here
        stats.addRow(date,"BUY",noShares,curPrice);
        flow.addRow(date,cashflow);
    }
    void sell(chrono::year_month_day date){
        noShares--;
        curBal = curBal + curPrice;
        stats.addRow(date,"SELL",noShares,curPrice);
        flow.addRow(date,cashflow);
    }
    void squareOff(chrono::year_month_day date){
        curBal = curBal + noShares*closePrice;
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
        if(startDateLoc == -1) { cout << "start date not located in the table for some reason" << endl;}
        
        int startDate_n_Loc = startDateLoc - n;
        double firstprice = table.rows[i].close;
        firstPrice(firstprice);

        for(int i=startDate_n_Loc+1;i<table.rows.size();i++){
            double newPrice = table.rows[i].close;
            chrono::year_month_day curDate = table.rows[i].date;
            nextPrice(newPrice,newDate);
        }
    }
}