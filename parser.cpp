#include <iostream>
#include <fstream> // needed to read file
#include <vector>
#include <sstream> // needed to parse
#include <chrono>
using namespace std;


chrono::year_month_day str2Date(string& date){
    int day = stoi(date.substr(0,2));
    int month = stoi(date.substr(3,2));
    int year = stoi(date.substr(6,4));
    return chrono::year_month_day(chrono::year(year),chrono::month(month),chrono::day(day));
}


class OrderStatsRow{
    public :
    chrono :: year_month_day Date ;
    string OrderDirection ;
    int Quantity ;
    double Price ;

    OrderStatsRow (string & line)
    {
        string word ;
        vector<string> a ;
        stringstream s(line) ;
        while(getline(s,word,','))
        {
            a.push_back(word) ;
        }
        Date = str2Date(a[0]) ;
        OrderDirection = a[1] ;
        Quantity = stoi(a[2]) ;
        Price = stod(a[3]) ;
    }

    string getString()
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
};

class OrderStats {
    public : 
    vector<OrderStatsRow> rows ; 
    string Header = "Date,Order_dir,Quantity,Price\n" ;
    void writeToCsv(string filename)
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
    void addRow(string& line)
    {   
        OrderStatsRow a = OrderStatsRow(line) ;
        rows.push_back(a) ;
    }
};

class CashFlowRows {
    public :
    chrono::year_month_day Date ;
    double Cashflow;
    CashFlowRows (string & line)
    {
        string word ;
        vector<string> a ;
        stringstream s(line) ;
        while(getline(s,word,','))
        {
            a.push_back(word) ;
        }
        Date = str2Date(a[0]) ;
        Cashflow = stod(a[1]) ;
    }
    string getString()
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
};

class CashFlow {
    public : 
    vector<CashFlowRows> rows ;
    string Header = "Date,Cashflow\n" ;

    void writeToCsv(string filename) 
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
    void addRow(string& line)
    {   
        CashFlowRows a = CashFlowRows(line) ;
        rows.push_back(a) ;
    }
};

class PriceTableRow{
    public :
    double price ;
    string date ;

    static PriceTableRow getRowObj(string row)
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
} ;

class PriceTable {
    public :
    vector<PriceTableRow> rows ;

    void read_csv(string filename)
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

} ;


int main()
{
    ifstream csv_file;
    string filename = "daily_pnl.csv";
    csv_file.open(filename);
    string line;
    string temp;
    string word;
    TableData CurrentTable;
    TableRow currentRow;
    while (getline(csv_file, line))
    {
        int i = 0;
        stringstream s(line);
        while (getline(s, word, ','))
        {
            switch (i)
            {
            case 0:
                currentRow.Date = word;
                i++;
                break;
            case 1:
                currentRow.Cashflow = word;
                i++;
                break;
            default:
                break;
            }
        }
        CurrentTable.Data.push_back(currentRow);
    }

    for (auto x : CurrentTable.Data)
    {
        cout << x.Date << " arpit " << x.Date << endl;
    }
    csv_file.close();

    filename = "newfile1.csv";
    TableData b;
    TableRow c;
    c.Cashflow = "3" ;
    c.Date = "04/01/2023" ;
    addToTableData(c , b ) ;
    writeToCsv(filename , b) ;
}