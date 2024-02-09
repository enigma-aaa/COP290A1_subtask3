#include "parser.h"
#include "dateUtil.h"
#include <cmath>
class MACD
{
    int x;
    int n1 = 12;
    int n2 = 26;
    int nSig = 9;
    double alpha1 = 2.0/(n1+1);
    double alpha2 = 2.0/(n2+1);
    double alphaSig = 2.0/(nSig+1);
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
    void first(int startDateLoc){
        int startDate_n1_Loc = startDateLoc - n1 + 1;
        int startDate_n2_Loc = startDateLoc - n2 + 1;
        int startDate_nSign_Loc = startDateLoc - nSign + 1;
        /* confirm that intially has to zero*/
        longEWM = 0;
        shortEWM = 0;
        /* have to recheck all the limits here may 
            require less than equal to instead of less than in many places*/
        /*again here also when including curDate do we take only n1-1 elements definetly confirm with sir*/
        for(int i=startDate_n1_Loc;i<startDate_n1_Loc +n1;i++){
            longEWM = alpha1*(table.rows[i].close - longEWM) + longEWM;
        }
        for(int i=startDate_n2_Loc;i<startDate_n2_Loc + n2;i++){
            shortEWM = alpha2*(table.rows[i].close - shortEWM) + shortEWM; 
        }
        /*Signal = 0 at time t= 0*/
        signal = 0;
        check();
    }
    void writeCashFlow(chrono::year_month_day curDate){
        flow.addRow(date,curBal);
    }
    void writeCSVfiles()
    {
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
        first(startDateLoc);
        //have to check start indices here also
        //handling startDateLoc already in check function have to start with startDateLoc+1
        for(int i = startDateLoc+1; i < table.rows.size();i++)
        {
            curPrice = table.rows[i].close;
            curDate = table.rows[i].date;
            longEWM = alpha1*(curPrice - longEWM) + longEWM;
            shortEWM = alpha2*(curPrice - shortEWM) + shortEWM;
            MACD = shortEWM - longEWM;
            signal = alphaSig*(MACD - signal) + signal;
            check();
            writeCashFlow(table.rows[i].date);
        }
        writeCSVfiles();
        squareOff();
        writeFinalPNL();
    }
};