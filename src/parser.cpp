#include "parser.h"
using namespace std;


chrono::year_month_day str2Date(string& date){
    int day = stoi(date.substr(0,2));
    int month = stoi(date.substr(3,2));
    int year = stoi(date.substr(6,4));
    return chrono::year_month_day(chrono::year(year),chrono::month(month),chrono::day(day));
}
chrono::year_month_day str2DateYMD(string& date){
    int year = stoi(date.substr(0,4));
    int month = stoi(date.substr(5,2));
    int day = stoi(date.substr(8,2));  
    return chrono::year_month_day(chrono::year(year),chrono::month(month),chrono::day(day)); 
}

string date2StrYMD(chrono::year_month_day date){
    string result = "";
    result += to_string(int(date.year())) + "/";
    result += to_string(unsigned(date.month())) + "/";
    result += to_string(unsigned(date.day()));
    return result;
}


OrderStatsRow::OrderStatsRow(chrono::year_month_day date , string orderDirection , int quantity , double price)
:date(date),orderDirection(orderDirection),quantity(quantity),price(price)
{}

string padNum(string input,int size){
    if(input.size() < size){
        int diff = size - input.size();
        string zeroes = string(diff,'0');
        return zeroes + input;
    }
    return input;
}
string OrderStatsRow::getString()
{
    string temp = ""; 
    temp+= padNum(to_string(unsigned(date.day())) ,2);
    temp+= '/' ;
    temp+= padNum(to_string(unsigned(date.month())),2) ;
    temp+= '/' ;
    temp+= to_string(int(date.year())) ;
    temp+=',' ;
    temp+=orderDirection ;
    temp+=',' ;
    temp+=to_string(quantity) ;
    temp+=',' ;
    temp+=to_string(price);
    temp+='\n' ;
    return temp ;
}  

void OrderStats::writeToCsv(string filename)
{   
    ofstream csv_file ;
    csv_file.open(filename) ;
    csv_file << Header ;
    string line;
    for(auto &x : rows)
    {
        line = x.getString() ;
        csv_file << line;
    }
    csv_file.close() ;
}
void OrderStats::addRow(chrono::year_month_day date , string orderDirection , int quantity , double price)
{
    //cout << "Trying to add row in order stats" << endl;
    OrderStatsRow a = OrderStatsRow(date , orderDirection , quantity , price) ;
    //cout << "created order stats object" << endl;
    rows.push_back(a) ;
    //cout << "added object to the row" << endl;
}


CashFlowRows::CashFlowRows(chrono::year_month_day date , double price)
:date(date),cashflow(price){}

string CashFlowRows::getString()
{
    string temp = ""; 
    temp+= to_string(unsigned(date.day()));
    temp+= '/' ;
    temp+= to_string(unsigned(date.month())) ;
    temp+= '/' ;
    temp+= to_string(int(date.year())) ;
    temp+=',' ;
    temp+=to_string(cashflow) ;
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
void DateFloat::writeToCsv(string filename){
    ofstream csv_file;
    csv_file.open(filename);
    string Header = "Date,"+floatHeading;
    string line;
    for(auto& x:rows)
    {
        line = x.getString();
        csv_file << line;
    }
    csv_file.close();
}
void CashFlow::addRow(chrono::year_month_day date , double curBal)
{
    CashFlowRows a = CashFlowRows(date,curBal) ;
    rows.push_back(a) ;
}

PriceTableRow PriceTableRow::getRowObj(string row)
{
    PriceTableRow temp ;
    stringstream s(row) ;
    string word ;
    vector<string> tokens;
    while(getline(s,word,','))
    {
        tokens.push_back(word) ;
    }
    temp.date = str2DateYMD(tokens[0]);
    temp.open = std::stod(tokens[1]);
    temp.high = std::stod(tokens[2]);
    temp.low = std::stod(tokens[3]);
    temp.prevClose = std::stod(tokens[4]);
    temp.close = std::stod(tokens[5]);
    temp.VWAP = std::stod(tokens[6]);
    temp.noTrades = std::stod(tokens[7]);
    return temp ;
}
string PriceTableRow::getString(){
    string result = "";
    result += date2StrYMD(date) + ",";
    result += to_string(open) + ",";
    result += to_string(high) + ",";
    result += to_string(low) + ",";
    result += to_string(prevClose) + ",";
    result += to_string(close) + ",";
    result += to_string(VWAP) + ",";
    result += to_string(noTrades) + ",";
    return result;
}
string DateFloatRow::getString(){
    string result = "";
    result += date2StrYMD(date) +",";
    result += to_string(val) + "\n";
    return result;
}

void PriceTable::read_csv(string filename)
{
    ifstream csv_file ;
    csv_file.open(filename) ;

    string line,word ;
    //ignoring header
    getline(csv_file,line);
    while(getline(csv_file,line))
    {
        PriceTableRow temp = PriceTableRow::getRowObj(line) ;
        rows.push_back(temp) ;
    }
}
/*
PriceTable::PriceTable(PriceTable&& other):rows(std::move(other.rows)){
    //cout << "called move constructor of price table" << endl;    
}
*/
PriceTable::PriceTable(const PriceTable& other):rows(other.rows){
    //cout << "called copy constructor" << endl;
}
PriceTable::PriceTable(){}