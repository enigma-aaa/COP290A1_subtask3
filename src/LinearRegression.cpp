#include "parser.h"
#include "dateUtil.h"
#include "CommonCons.h"
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
    double alpha ;

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
        alpha = 0.001 ;
    }


    void fit()
    {
        train_table = getPriceTable(symbolName , train_start_date , train_end_date) ;
        
        for(int iter = 0 ; iter<10000 ; iter++)
        {
            vector<double> multFactors(8,0.0) ;
            for(int i = 1 ; i<train_table.rows.size() ; i++)
            {
                double cur_pred_price = 0.0 ;
                cur_pred_price += coefficients[0] ;
                cur_pred_price += train_table.rows[i-1].close * coefficients[1] ;
                cur_pred_price += train_table.rows[i-1].open * coefficients[2];
                cur_pred_price += train_table.rows[i-1].VWAP * coefficients[3];
                cur_pred_price += train_table.rows[i-1].low * coefficients[4];
                cur_pred_price += train_table.rows[i-1].high * coefficients[5];
                cur_pred_price += train_table.rows[i-1].noTrades * coefficients[6];
                cur_pred_price += train_table.rows[i].open * coefficients[7];  

                double pred_minus_label = cur_pred_price - train_table.rows[i].close ;

                multFactors[0] = pred_minus_label ; 
                multFactors[1] += pred_minus_label * train_table.rows[i-1].close ;
                multFactors[2] += pred_minus_label * train_table.rows[i-1].open ;
                multFactors[3] += pred_minus_label * train_table.rows[i-1].VWAP ;
                multFactors[4] += pred_minus_label * train_table.rows[i-1].low ;
                multFactors[5] += pred_minus_label * train_table.rows[i-1].high ;
                multFactors[6] += pred_minus_label * train_table.rows[i-1].noTrades ;
                multFactors[7] += pred_minus_label * train_table.rows[i-1].open ;
            
            }

            for(int i = 0 ; i < 8 ; i++)
            {
                coefficients[i] = coefficients[i] - (alpha/7)*multFactors[i] ;
            }
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
        table = getPriceTable(symbolName , mod_start_date , end_date) ;
        int startDateLoc = -1 ;
        for(int i = 0 ; i<table.rows.size() ; i++)
        {
            if(table.rows[i].date == start_date)
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