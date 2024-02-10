#include "parser.h"
#include "dateUtil.h"
#include <cmath>

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
    chrono::year_month_day end_date , double p , int x , string symbolName)
    {
        train_start_date = train_start_date ;
        train_end_date = train_end_date ;
        start_date = start_date ;
        end_date = end_date ;
        p = p;
        x = x ;
        symbolName = symbolName ;
        coefficients.resize(8,0.0) ;
    }

    vector<vector<double>> inverse(vector<vector<double>>& v)
    {   
        int n = v.size() ;


        vector<vector<double>> augmentedMatrix ;
        augmentedMatrix.resize(n,vector<double>(2*n,0.0)) ;


        for(int i = 0 ; i<n ; i++)
        {
            for(int j = 0  ; j<2*n ; j++)
            {
                augmentedMatrix[i][j] = augmentedMatrix[i][j]/augmentedMatrix[i][i] ;
            }
            for(int j = 0 ;j<n ; j++)
            {
                if(j != i )
                {
                    double div = augmentedMatrix[j][i] ;
                    for(int k = 0 ;  k<2*n ; k++)
                    {
                        augmentedMatrix[j][k] = augmentedMatrix[j][k] - div * augmentedMatrix[i][k] ;
                    }
                }
            }
        }

        vector<vector<double>> ans(n,vector<double>(n,0.0)) ;

        for(int i = 0 ; i<n ; i++)
        {
            for(int j =0  ; j<n ; j++)
            {
                ans[i][j] = augmentedMatrix[i][j+n] ;
            }
        }

        return ans ;
    }

    vector<vector<double>> transpose(vector<vector<double>>& v)
    {
        vector<vector<double>> ans ;
        for(int j = 0 ; j<v[0].size() ; j++)
        {
            vector<double> temp ;
            for(int i = 0 ; i<v.size() ; i++)
            {
                temp.push_back(v[i][j]) ;
            }
            ans.push_back(temp) ;
        }
        return ans ;

    }

    vector<vector<double>> matrixmult(vector<vector<double>> &a , vector<vector<double>>&b)
    {
        vector<vector<double>> ans(a.size(),vector<double>(b[0].size()) );


        for(int i = 0 ; i<a.size() ; i++)
        {
            for(int j = 0 ; j<b[0].size() ; j++)
            {
                for(int k = 0  ; k<a[0].size() ; k++)
                {
                    ans[i][j] += a[i][k] * b[k][j] ;
                }
            }
        }
        return ans ;
    }

    void fit()
    {
        train_table = getPriceTable(symbolName , train_start_date , train_end_date) ;
        vector<vector<double>> X ;
        vector<double> temp ;
        for(int i = 1 ; i<train_table.rows.size() ; i++)
        {
            temp.clear() ;
            temp.push_back(1.0) ;
            temp.push_back(train_table.rows[i-1].close) ;
            temp.push_back(train_table.rows[i-1].open) ;
            temp.push_back(train_table.rows[i-1].VWAP) ;
            temp.push_back(train_table.rows[i-1].low) ;
            temp.push_back(train_table.rows[i-1].high) ;
            temp.push_back(train_table.rows[i-1].noTrades) ;
            temp.push_back(train_table.rows[i].open) ;
            X.push_back(temp) ;
        }

        vector<vector<double>> Y ;

        for(int i = 1 ; i<train_table.rows.size() ; i++)
        {
            Y.push_back({train_table.rows[i].close}) ;
        }

        vector<vector<double>> Xt = transpose(X) ;
        vector<vector<double>> XXt = matrixmult(Xt,X) ;
        vector<vector<double>>XXtInv = inverse(XXt) ;
        vector<vector<double>>tempAns = matrixmult(XXtInv , Xt) ;
        vector<vector<double>> ans = matrixmult(tempAns,Y) ;

        for(int i = 0 ; i<coefficients.size() ; i++)
        {
            coefficients[i] = ans[i][0] ;
        }
    }

    double predict(int tableLoc)
    {
        double ans = 0 ;
        ans += coefficients[0] ;
        ans += coefficients[1] * table.rows[tableLoc-1].close;
        ans += coefficients[2] * table.rows[tableLoc-1].open;
        ans += coefficients[3] * table.rows[tableLoc-1].VWAP;
        ans += coefficients[4] * table.rows[tableLoc-1].low;
        ans += coefficients[5] * table.rows[tableLoc-1].high;
        ans += coefficients[6] * table.rows[tableLoc-1].noTrades;
        ans += coefficients[7] * table.rows[tableLoc].open;
        return ans ;
    }

    void buy()
    {
        noShares++ ;
        curBal = curBal - curPrice ;
        stats.addRow(table.rows[curLoc].date , "BUY" , noShares , curPrice) ;
        flow.addRow(table.rows[curLoc].date, curBal) ;
    }
    void sell()
    {
        noShares-- ;
        curBal = curBal + curPrice ;
        stats.addRow(table.rows[curLoc].date , "SELL" , noShares , curPrice) ;
        flow.addRow(table.rows[curLoc].date, curBal) ;
    }

    void check()
    {
        double predicted_price = predict(curLoc) ;
        if(predicted_price >= curPrice*(1+p/100))
        {
            if(noShares <x)
            {
                buy() ;
            }
        }
        if(predicted_price <= curPrice*(1-p/100))
        {
            if(noShares>-1*x)
            {
                sell() ;
            }
        }
    }


    void squareOff()
    {
        curBal = curBal + noShares*table.rows.back().close ;
        if(noShares > 0 )
        {
            stats.addRow(table.rows.back().date, "SELL" , noShares ,curPrice) ;
            flow.addRow(table.rows.back().date , curBal) ;
        }
        if(noShares < 0)
        {
            stats.addRow(table.rows.back().date, "BUY" , noShares , curPrice) ;
            flow.addRow(table.rows.back().date , curBal) ;
        }
        noShares = 0 ;
    }
    void main()
    {
        chrono:: year_month_day mod_start_date = subtractDate(start_date , 5) ;
        table = getPriceTable(symbolName , mod_start_date , end_Date) ;
        int startDateLoc = -1 ;
        for(int i = 0 ; i<table.rows.size() ; i++)
        {
            if(table.rows[i].date = start_date)
            {
                startDateLoc = i ;
            }
        }

        for(int i =startDateLoc ; i<table.rows.size()-1 ; i++)
        {
            curPrice = table.rows[i].close ;
            curLoc = i;
            check() ;
        }
        squareOff() ;

    }


}; 