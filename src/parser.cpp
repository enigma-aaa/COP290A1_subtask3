#include "parser.h"
using namespace std;


chrono::year_month_day str2Date(string& date){
    int day = stoi(date.substr(0,2));
    int month = stoi(date.substr(3,2));
    int year = stoi(date.substr(6,4));
    return chrono::year_month_day(chrono::year(year),chrono::month(month),chrono::day(day));
}



OrderStatsRow::OrderStatsRow(chrono::year_month_day date , string OrderDirection , int quantity , double price)
{
    Date = date ,
    OrderDirection = OrderDirection ;
    Quantity = quantity ;
    Price = price ;
}
OrderStatsRow::getString()
{
    string temp = ""; 
    temp+= to_string(int(Date.day));
    temp+= '/' ;
    temp+= to_string(int(Date.month)) ;
    temp+= '/' ;
    temp+= to_string(int(Date.year)) ;
    temp+=',' ;
    temp+=OrderDirection ;
    temp+=',' ;
    temp+=to_string(Quantity) ;
    temp+=',' ;
    temp+=to_string(Price);
    temp+='\n' ;
    return temp ;
}  

void OrderStats::writeToCsv(string filename)
{   
    ofstream csv_file ;
    csv_file.open(filename) ;
    csv_file<<Header ;
    string line ;
    for(auto &x : rows)
    {
        string temp = x.getString() ;
        csv_file<<temp ;
    }
    csv_file.close() ;
}
void OrderStats::addRow(chrono::year_month_date date , string orderDirection , int quantity , double price)
{
    OrderStatsRow a = OrderStatsRow(date , orderDirection , quantity , price) ;
    rows.push_back(a) ;
}


CashFlowRows::CashFlowRows(chrono::year_month_day date , double price)
{
    Date = date ;
    Cashflow = price ;
}
string CashFlowRows::getString()
{
    string temp = ""; 
    temp+= to_string(int(Date.day));
    temp+= '/' ;
    temp+= to_string(int(Date.month)) ;
    temp+= '/' ;
    temp+= to_string(int(Date.year)) ;
    temp+=',' ;
    temp+=to_string(Cashflow) ;
    temp+='\n' ;
    return temp ;
}


void CashFlow::writeToCsv(string filename) 
{
    ofstream csv_file ;
    csv_file.open(filename) ;
    csv_file << Header ;
    string line ;
    for(auto & x : rows)
    {
        line = x.getString() ;
        csv_file<<line ;
    }    
    csv_file.close() ;
}
void CashFlow::addRow(chrono::year_month_day date , double price)
{
    CashFlowRows a = CashFlowRows(date,price) ;
    rows.push_back(a) ;
}

static PriceTableRow PriceTableRow::getRowObj(string row)
{
    PriceTableRow temp ;
    stringstream s(row) ;
    string word ;
    vector<string> a ;
    while(getline(s,word,','))
    {
        a.push_back(word) ;
    }
    temp.date = a[1] ;
    temp.price = double(stod(a[0])) ;
    return temp ;
}

void PriceTable::read_csv(string filename)
{
    ifstream csv_file ;
    csv_file.open(filename) ;

    string line,word ;
    while(getline(csv_file,line))
    {
        PriceTableRow temp = PriceTableRow :: getRowObj(line) ;
        rows.push_back(temp) ;
    }
}


int main()
{
    
}