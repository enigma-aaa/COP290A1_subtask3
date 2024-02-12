#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
using namespace std;

chrono::year_month_day str2Date(string& date);

class OrderStatsRow{
    public :
        chrono :: year_month_day date ;
        string orderDirection ;
        int quantity ;
        double price ;

    OrderStatsRow(chrono::year_month_day date , string orderDirection , int quantity , double price);
    string getString();
};

class OrderStats {
    public : 
        vector<OrderStatsRow> rows ; 
        string Header = "Date,Order_dir,Quantity,Price\n" ;
    void writeToCsv(string filename);
    void addRow(chrono::year_month_day date , string orderDirection , int quantity , double price);
};
class CashFlowRows {
    public :
        chrono::year_month_day date ;
        double cashflow;
    CashFlowRows(chrono::year_month_day date , double price);
    string getString();
};

class CashFlow {
    public : 
        vector<CashFlowRows> rows ;
        string Header = "Date,Cashflow\n" ;

    void writeToCsv(string filename);
    void addRow(chrono::year_month_day date , double cashflow);
};

class PriceTableRow{
    public:
        chrono::year_month_day date;
        double open;
        double high;
        double low;
        double prevClose;
        double close;
        double VWAP;
        double noTrades;
    static PriceTableRow getRowObj(string row);
    string getString();
};

class PriceTable {
    public :
        vector<PriceTableRow> rows ;
    PriceTable();
    PriceTable(const PriceTable& other);
    void read_csv(string filename);
} ;

class DateFloatRow{
public: 
    chrono::year_month_day date;
    float val;
    DateFloatRow(chrono::year_month_day date,float val);
    string getString();
};
class DateFloat{
    public:
        vector<DateFloatRow> rows;
        string floatHeading;
    DateFloat();
    DateFloat(string floatHeading);
    void writeToCsv(string filename);
};