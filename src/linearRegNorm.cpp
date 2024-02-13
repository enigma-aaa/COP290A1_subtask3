#include "LinearRegNorm.h"
#include "CommonCons.h"

LinearRegression::LinearRegression(chrono::year_month_day train_start_date, chrono::year_month_day train_end_date, chrono ::year_month_day start_date,
                                   chrono::year_month_day end_date, double p, int x, string symbolName):x(x) , p(p) , symbolName(symbolName) ,train_start_date(train_start_date),
                                   train_end_date(train_end_date) , start_date(start_date) , end_date(end_date)
{
    coefficients.resize(8, 0.0);
    noShares = 0 ;
    curBal = 0 ;
    curLoc = -1 ;
    mod_start_date = subtractDate(start_date, 10);
}
LinearRegression::LinearRegression(){}
vector<vector<double>> LinearRegression::inverse(vector<vector<double>> &v)
{
    int n = v.size();

    vector<vector<double>> augmentedMatrix;
    augmentedMatrix.resize(n, vector<double>(2 * n, 0.0));
    for(int i = 0 ; i<n ; i++)
    {
        for(int j = 0 ; j <n ; j++)
        {
            if(i == j)
            {
                augmentedMatrix[i][j+n] = 1.0 ;
            }
            augmentedMatrix[i][j] = v[i][j] ;
        }
    }

    for(int i = 0 ; i<n ; i++)
    {
        double temp = augmentedMatrix[i][i] ;
        for (int j = 0; j < 2 * n; j++)
        {
            augmentedMatrix[i][j] = augmentedMatrix[i][j] / temp;
        }
        for (int j = 0; j < n; j++)
        {
            if (j != i)
            {
                double div = augmentedMatrix[j][i]/augmentedMatrix[i][i];
                for (int k = 0; k < 2 * n; k++)
                {
                    
                    augmentedMatrix[j][k] = augmentedMatrix[j][k] - div * augmentedMatrix[i][k];
                }
            }
        }

    }




    vector<vector<double>> ans(n, vector<double>(n, 0.0));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            ans[i][j] = augmentedMatrix[i][j + n];
        }
    }

    return ans;
}

vector<vector<double>> LinearRegression::transpose(vector<vector<double>> &v)
{
    vector<vector<double>> ans;
    // cout<<"inside transpose "<<v.size()<<endl;
    for (int j = 0; j < v[0].size(); j++)
    {
        vector<double> temp;
        for (int i = 0; i < v.size(); i++)
        {
            temp.push_back(v[i][j]);
        }
        ans.push_back(temp);
    }
    return ans;
}

vector<vector<double>> LinearRegression::matrixmult(vector<vector<double>> &a, vector<vector<double>> &b)
{
    vector<vector<double>> ans(a.size(), vector<double>(b[0].size()));

    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < b[0].size(); j++)
        {
            for (int k = 0; k < a[0].size(); k++)
            {
                ans[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return ans;
}

void printMatrix(vector<vector<double>>& matrix,string pnlFileName){
    ofstream file(pnlFileName);
    for(int i=0;i<matrix.size();i++){
        for(int j=0;j<matrix[i].size();j++){
            file << matrix[i][j];
        }
        file << endl;
    }
    file.close();
}

void LinearRegression::fit()
{
    chrono::year_month_day modified_train_start_date = subtractDate(train_start_date , 10) ;
    train_table = getPriceTable(symbolName, modified_train_start_date, train_end_date);
    int startLoc= train_table.rows.size();

    for(int i = 0 ; i<train_table.rows.size() ; i++)
    {
        if(grtrEqual(train_table.rows[i].date,train_start_date)){
            startLoc = i;
            //cout << "startDateLOc is:" << startLoc << endl;
            break;
        }
    }
    vector<vector<double>> X;
    //cout << "startLoc in linear Regression is:" << startLoc << endl;

    vector<double> temp;

    for (int i = startLoc; i < train_table.rows.size(); i++)
    {
        temp.clear();
        temp.push_back(1.0);
        temp.push_back(train_table.rows[i - 1].close);
        temp.push_back(train_table.rows[i - 1].open);
        temp.push_back(train_table.rows[i - 1].VWAP);
        temp.push_back(train_table.rows[i - 1].low);
        temp.push_back(train_table.rows[i - 1].high);
        temp.push_back(train_table.rows[i - 1].noTrades);
        temp.push_back(train_table.rows[i].open);
        X.push_back(temp);

    }

    vector<vector<double>> Y;

    for (int i = startLoc; i < train_table.rows.size(); i++)
    {
        Y.push_back({train_table.rows[i].close});
    }

    // for(auto x : X)   
    // {
    //     for(auto y : x)
    //     {
    //         cout<<y<<" ";
    //     }
    //     cout<<endl;
    // }


    vector<vector<double>> Xt = transpose(X);


    vector<vector<double>> XXt = matrixmult(Xt, X);

    vector<vector<double>> XXtInv = inverse(XXt);

    vector<vector<double>> check1 = matrixmult(XXtInv,XXt) ;
    printMatrix(XXtInv,"XXtinv.txt");




    vector<vector<double>> tempAns = matrixmult(XXtInv, Xt);
    printMatrix(tempAns,"tempAns.txt");

    vector<vector<double>> ans = matrixmult(tempAns, Y);
    printMatrix(ans,"finalAns.txt");
    
    for (int i = 0; i < coefficients.size(); i++)
    {
        coefficients[i] = ans[i][0];
        //cout<<coefficients[i]<<" ";
    }
    //cout<<endl;
}

double LinearRegression::predict(int tableLoc)
{
    double ans = 0;
    ans += coefficients[0];
    ans += coefficients[1] * table->rows[tableLoc - 1].close;
    ans += coefficients[2] * table->rows[tableLoc - 1].open;
    ans += coefficients[3] * table->rows[tableLoc - 1].VWAP;
    ans += coefficients[4] * table->rows[tableLoc - 1].low;
    ans += coefficients[5] * table->rows[tableLoc - 1].high;
    ans += coefficients[6] * table->rows[tableLoc - 1].noTrades;
    ans += coefficients[7] * table->rows[tableLoc].open;
    return ans;
}

void LinearRegression::buy()
{
    noShares++;
    curBal = curBal - curPrice;
    stats.addRow(table->rows[curLoc].date, "BUY", 1, curPrice);
}
void LinearRegression::sell()
{
    noShares--;
    curBal = curBal + curPrice;
    stats.addRow(table->rows[curLoc].date, "SELL", 1, curPrice);
}

void LinearRegression::check()
{
    double predicted_price = predict(curLoc);
    // cout<<predicted_price<<" "<<curPrice<<endl;
    if (predicted_price >= (curPrice + curPrice * p / 100))
    {
        if (noShares < x)
        {
            buy();
        }
    }
    if (predicted_price <= (curPrice  - curPrice * p / 100))
    {
        if (noShares > -1 * x)
        {
            sell();
        }
    }
}
void LinearRegression::writeCSVfiles()
{
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats = baseFilePath + ORDER_STATS;
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);
}


void LinearRegression::writeFinalPNL(){
    stringstream stream;
    stream << std::fixed << std::setprecision(2) << curBal;
    string curBalStr = stream.str();
    string baseFilePath = BASE_FILE_PATH;
    string pnlFileName = FINAL_PNL;
    string pnlFilePath = baseFilePath + pnlFileName;
    ofstream pnlFile(pnlFilePath);
    pnlFile << curBalStr;
    pnlFile.close();
}
void LinearRegression::squareOff()
{
    curBal = curBal + noShares * table->rows.back().close;
    noShares = 0;
}
void LinearRegression::main()
{
    fit() ;
    //for(auto x : coefficients)
    //{
    //    cout<<x<<" ";
    //}
    //cout<<endl;
    PriceTable createTable = getPriceTable(symbolName, mod_start_date, end_date);
    multiMain(&createTable);
    writeCSVfiles() ;
    writeFinalPNL();
}

void LinearRegression::multiMain(PriceTable* srcTable){
    table = srcTable;

    int startDateLoc = table->rows.size();
    for (int i = 0; i < table->rows.size(); i++)
    {
        if(grtrEqual(table->rows[i].date,start_date)){
            startDateLoc = i;
            break;
        }
    }

    for (int i = startDateLoc; i < table->rows.size(); i++)
    {
        curPrice = table->rows[i].close;
        curLoc = i;
        check();
        flow.addRow(table->rows[curLoc].date, curBal);
    }
    squareOff();
}