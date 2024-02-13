#include <iostream>
#include <string>
#include <chrono>
#include "parser.h"
#include "dateUtil.h"
#include "ADX.h"
#include "basicStart.h"
#include "DMA.h"
#include "DMAplus.h"
#include "MACD.h"
#include "RSI.h"
#include "Best.h"
#include "Pairs.h"
#include "PairsStopLoss.h"
#include <cstdlib>
#include "LinearRegNorm.h"
using namespace std;

// path in terminal here same as from where the exe is run

void handleBASIC(int argc, char *argv[])
{
    string symbolName(argv[2]);
    int n = stoi(argv[3]);
    int x = stoi(argv[4]);
    string startDateStr(argv[5]);
    chrono::year_month_day startDate = str2Date(startDateStr);
    string endDateStr(argv[6]);
    chrono::year_month_day endDate = str2Date(endDateStr);

    Basic strat(n, x, startDate, endDate, symbolName);
    strat.main();
    // cout << "symbol is:" << symbol << endl;
    // cout << "n is:" << n << endl;
    // cout << "x is:" << x << endl;
    // cout << "startDate str is:" << startDateStr << endl;
    // cout << "endDate str is:" << endDateStr << endl;
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
}
void handleDMA(int argc, char *argv[])
{
    string symbolName(argv[2]);
    int n = stoi(argv[3]);
    int x = stoi(argv[4]);
    int p = stoi(argv[5]);
    string startDateStr(argv[6]);
    chrono::year_month_day startDate = str2Date(startDateStr);
    string endDateStr(argv[7]);
    chrono::year_month_day endDate = str2Date(endDateStr);

    DMA strat(n, x, p, startDate, endDate, symbolName);
    strat.main();
    // cout << "symbol is:" << symbol << endl;
    // cout << "n is:" << n << endl;
    // cout << "x is:" << x << endl;
    // cout << "p is:" << p << endl;
    // cout << "startDate str is:" << startDateStr << endl;
    // cout << "endDate str is:" << endDateStr << endl;
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
}
void handleDMAplus(int argc, char *argv[])
{
    // cout << "Entered DMA++" << endl;
    string symbol(argv[2]);
    // cout << "symbol is:" << symbol << endl;
    int x = stoi(argv[3]);
    // cout << "x is:" << x << endl;
    int p = stoi(argv[4]);
    // cout << "p is:" << p << endl;
    int n = stoi(argv[5]);
    // cout << "n is:" << n << endl;
    int max_hold_days = stoi(argv[6]);
    // cout << "max_hold_days:" << max_hold_days << endl;
    float c1 = stof(argv[7]);
    // cout << "c1:" << c1 << endl;
    float c2 = stof(argv[8]);
    // cout << "c2:" << c2 << endl;
    // cout << "argc is:" << argc << endl;
    // cout << "argv[9]:" << argv[9] << " argv[10]:" << argv[10] << endl;
    string startDateStr(argv[9]);
    // cout << "startDate str is:" << startDateStr << endl;
    string endDateStr(argv[10]);
    // cout << "endDate str is:" << endDateStr << endl;
    chrono::year_month_day startDate = str2Date(startDateStr);
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    chrono::year_month_day endDate = str2Date(endDateStr);
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
    DMAPlus strat(n, x, p, max_hold_days, c1, c2, startDate, endDate, symbol);
    strat.main();
}
void handleMACD(int argc, char *argv[])
{
    string symbol(argv[2]);
    int x = stoi(argv[3]);
    string startDateStr(argv[4]);
    string endDateStr(argv[5]);
    chrono::year_month_day startDate = str2Date(startDateStr);
    chrono::year_month_day endDate = str2Date(endDateStr);
    // cout << "symbol is:" << symbol << endl;
    // cout << "x is:" << x << endl;
    // cout << "startDate str is:" << startDateStr << endl;
    // cout << "endDate str is:" << endDateStr << endl;
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
    MACDStrat strat(x, startDate, endDate, symbol);
    strat.main();
}
void handleRSI(int argc, char *argv[])
{
    string symbolName(argv[2]);
    // cout << "symbol is:" << symbol << endl;
    int x = stoi(argv[3]);
    // cout << "x is:" << x << endl;
    int n = stoi(argv[4]);
    // cout << "n is:" << n << endl;
    int oversold_threshold = stoi(argv[5]);
    // cout << "oversold is:" << oversold << endl;
    int overbought_threshold = stoi(argv[6]);
    // cout << "overbought is:" << overbought << endl;
    string startDateStr(argv[7]);
    // cout << "startDate str is:" << startDateStr << endl;
    string endDateStr(argv[8]);
    // cout << "endDate str is:" << endDateStr << endl;
    chrono::year_month_day startDate = str2Date(startDateStr);
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    chrono::year_month_day endDate = str2Date(endDateStr);
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
    RSI strat(n, x, oversold_threshold, overbought_threshold, startDate, endDate, symbolName);
    strat.main();
}
void handleADX(int argc, char *argv[])
{
    string symbol(argv[2]);
    int x = stoi(argv[3]);
    int n = stoi(argv[4]);
    int adx_threshold = stoi(argv[5]);
    string startDateStr(argv[6]);
    string endDateStr(argv[7]);
    chrono::year_month_day startDate = str2Date(startDateStr);
    chrono::year_month_day endDate = str2Date(endDateStr);
    // cout << "symbol is:" << symbol << endl;
    // cout << "x is:" << x << endl;
    // cout << "n is:" << n << endl;
    // cout << "adx_threshold is:" << adx_threshold << endl;
    // cout << "startDate str is:" << startDateStr << endl;
    // cout << "endDate str is:" << endDateStr << endl;
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
    ADXStrat strat(n, x, adx_threshold, startDate, endDate, symbol);
    strat.main();
}
void handleRegression(int argc, char *argv[])
{
    string symbol(argv[2]);
    // cout << "symbol is:" << symbol << endl;
    int x = stoi(argv[3]);
    // cout << "x is:" << x << endl;
    int p = stoi(argv[4]);
    // cout << "p is:" << p << endl;
    string trainStartStr(argv[5]);
    // cout << "trainStartStr is:" << trainStartStr << endl;
    string trainEndStr(argv[6]);
    // cout << "trainEndStr is:" << trainEndStr << endl;
    string startDateStr(argv[7]);
    // cout << "startDate str is:" << startDateStr << endl;
    string endDateStr(argv[8]);
    // cout << "endDate str is:" << endDateStr << endl;
    chrono::year_month_day trainStart = str2Date(trainStartStr);
    // cout << "trainStart is:" << unsigned(trainStart.day()) << "/" << unsigned(trainStart.month()) << "/" << int(trainStart.year()) << endl;
    chrono::year_month_day trainEnd = str2Date(trainEndStr);
    // cout << "trainEnd is:" << unsigned(trainEnd.day()) << "/" << unsigned(trainEnd.month()) << "/" << int(trainEnd.year()) << endl;
    chrono::year_month_day startDate = str2Date(startDateStr);
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    chrono::year_month_day endDate = str2Date(endDateStr);
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;

    LinearRegression strat(trainStart, trainEnd, startDate, endDate, p, x, symbol);
    strat.main();
}
void handleBestOfAll(int argc, char *argv[])
{
    string symbol(argv[2]);
    string startDateStr(argv[3]);
    string endDateStr(argv[4]);
    chrono::year_month_day startDate = str2Date(startDateStr);
    chrono::year_month_day endDate = str2Date(endDateStr);
    Best best(startDate, endDate, symbol);
    // cout << "startDate str is:" << startDateStr << endl;
    // cout << "endDate str is:" << endDateStr << endl;
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
    best.main();
}
void handlePairs(int argc, char *argv[])
{
    string symbol1(argv[2]);
    string symbol2(argv[3]);
    int x = stoi(argv[4]);
    int n = stoi(argv[5]);
    double threshold = stod(argv[6]);
    string startDateStr(argv[7]);
    string endDateStr(argv[8]);
    chrono::year_month_day startDate = str2Date(startDateStr);
    chrono::year_month_day endDate = str2Date(endDateStr);
    // cout << "symbol1:" << symbol1 << endl;
    // cout << "symbol2:" << symbol2 << endl;
    // cout << "x:" << x << endl;
    // cout << "n:" << n << endl;
    // cout << "threshold:" << threshold << endl;
    // cout << "startDateStr:" << startDateStr << endl;
    // cout << "endDateStr:" << endDateStr << endl;
    // cout << "startDate is:" << unsigned(startDate.day()) << "/" << unsigned(startDate.month()) << "/" << int(startDate.year()) << endl;
    // cout << "endDate is:" << unsigned(endDate.day()) << "/" << unsigned(endDate.month()) << "/" << int(endDate.year()) << endl;
    if (argc == 9)
    {
        Pairs pairs(x, n, threshold, startDate, endDate, symbol1, symbol2);
        pairs.main();
    }
    else if (argc == 10)
    {
        int stop_loss_thres = stoi(argv[9]);
        PairsStopLoss pairsStopLoss(x, n, threshold, stop_loss_thres, startDate, endDate, symbol1, symbol2);
        pairsStopLoss.main();
        // cout << "stop_loss_thres:" << stop_loss_thres << endl;
    }
    else
    {
        // cout << "Wrong number of arguments for pair" << endl;
    }
}

int main(int argc, char *argv[])
{
    string startegyName(argv[1]);
    if (startegyName == "BASIC")
    {
        handleBASIC(argc, argv);
    }
    else if (startegyName == "DMA")
    {
        handleDMA(argc, argv);
    }
    else if (startegyName == "DMA++")
    {
        handleDMAplus(argc, argv);
    }
    else if (startegyName == "MACD")
    {
        handleMACD(argc, argv);
    }
    else if (startegyName == "RSI")
    {
        handleRSI(argc, argv);
    }
    else if (startegyName == "ADX")
    {
        handleADX(argc, argv);
    }
    else if (startegyName == "LINEAR_REGRESSION")
    {
        handleRegression(argc, argv);
    }
    else if (startegyName == "BEST_OF_ALL")
    {
        handleBestOfAll(argc, argv);
    }
    else if (startegyName == "PAIRS")
    {
        handlePairs(argc, argv);
    }
    else
    {
        // cout << "Invalid strategy" << endl;
    }
}

/*
int main(){
    string symbolName = "TATASTEEL";
    string startDateStr = "01-02-2024";
    string endDateStr = "06-02-2024";
    chrono::year_month_day startDate = str2Date(startDateStr);
    chrono::year_month_day endDate = str2Date(endDateStr);
    PriceTable table = getPriceTable(symbolName,startDate,endDate);
}
*/