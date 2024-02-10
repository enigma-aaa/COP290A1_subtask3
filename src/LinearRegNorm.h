#include "parser.h"
#include "dateUtil.h"
#include <cmath>
#include <iomanip>
class LinearRegression{

    public :
    chrono::year_month_day train_start_date , train_end_date , start_date , end_date ;
    double p ;
    int x ;
    vector<double> coefficients ;
    string symbolName ;

    PriceTable train_table ;
    PriceTable table ;
    CashFlow flow ;
    OrderStats stats ;

    int noShares ;
    double curBal ;
    double curPrice ;
    int curLoc ;

    LinearRegression(chrono::year_month_day train_start_date , chrono::year_month_day train_end_date , chrono ::year_month_day start_date,
    chrono::year_month_day end_date , double p , int x , string symbolName) ;
    vector<vector<double>> inverse(vector<vector<double>>& v) ;
    vector<vector<double>> transpose(vector<vector<double>>& v) ;
    vector<vector<double>> matrixmult(vector<vector<double>> &a , vector<vector<double>>&b) ;

    void fit() ;
    double predict(int tableLoc) ;
    void buy() ;
    void sell() ;
    void check();
    void squareOff() ;
    void main() ;
    void writeCSVfiles() ;
    void writeFinalPNL() ;
};