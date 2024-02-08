#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
using namespace std;
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
    void addRow(chrono::year_month_date date , string orderDirection , int quantity , double price);
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
    void addRow(chrono::year_month_day date , double price);
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
};

class PriceTable {
    public :
        vector<PriceTableRow> rows ;
    void read_csv(string filename);
} ;