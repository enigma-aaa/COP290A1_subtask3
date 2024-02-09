#include "dateUtil.h"
#include <chrono>
#include <sstream>
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
        int curLoc;

    Basic(int n,int x,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName):
    n(n),x(x),startDate(startDate),endDate(endDate),symbolName(symbolName){
        modStartDate = subtractDate(startDate,2*n);
    }
    /*first price opver here is not the price at the start 
        date but price n days before the start date*/
    void firstPrice(double orgPrice){
        curPrice = orgPrice;
        noIncDays = 0;
        noDecDays = 0;
        curBal = 0;
        noShares = 0;
    }
    void nextPrice(double newPrice,chrono::year_month_day newDate){
        double oldPrice = curPrice;
        curPrice = newPrice;
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
        if(newPrice == curPrice){
            noIncDays = 0;
            noDecDays = 0;            
        }
    }    
    void buy(chrono::year_month_day date){  
        noShares++;
        curBal = curBal - curPrice;
        //Assuming showing new quantity here
        stats.addRow(date,"BUY",noShares,curPrice);
    }
    void sell(chrono::year_month_day date){
        noShares--;
        curBal = curBal + curPrice;
        stats.addRow(date,"SELL",noShares,curPrice);
    }
    /*Ensure price set to closing price before calling*/
    void squareOff(chrono::year_month_day date){
        curBal = curBal + noShares*curPrice;
        noShares = 0;
    }
    void writeCashFlow(chrono::year_month_day curDate){
        flow.addRow(curDate,curBal);
    }
    void writeCSVfiles(){
        string baseFilePath = "./bin/stockCSV/"
        string csv_cashflow = baseFilePath + "cashflow.csv";
        string csv_order_stats = baseFilePath + "order_stats.csv";
        cashflow.writeToCsv(csv_cashflow);
        stats.writeToCsv(csv_order_stats);
    }
    void writeFinalPNL(){
        stringstream stream;
        stream << std::fixed << std::setprecision(2) << curBal;
        string curBalStr = stream.str();
        string baseFilePath = "./bin/stockCSV/"
        string pnlFileName = "finalPNL.txt";
        string pnlFilePath = baseFilePath + pnlFileName;
        ofstream pnlFile(pnlFilePath);
        pnlFile << curBalStr;
        pnlFile.close();
    }
    void main(string symbolName){
        table = gectPriceTable(symbolName,modStartDate,endDate);
        int tableSize = table.rows.size();
        int startDateLoc = -1;
        for(int i=0;i<table.rows.size();i++){
            if(table.rows[i].date == startDate){
                startDateLoc = i;
            }
        }
        if(startDateLoc == -1) { cout << "start date not located in the table for some reason" << endl;}
        
        int startDate_n_Loc = startDateLoc - n;
        double firstprice = table.rows[startDate_n_Loc].close;
        firstPrice(firstprice);

        for(int i=startDate_n_Loc+1;i<table.rows.size();i++){
            double newPrice = table.rows[i].close;
            chrono::year_month_day curDate = table.rows[i].date;
            nextPrice(newPrice,newDate);
            writeCashFlow();
        }
        writeCSVfiles();
        squareOff(table.rows.back());
        writeFinalPNL();
    }
}