#pragma once
#include "parser.h"
#include "dateUtil.h"
#include "ADX.h"
#include "basicStart.h"
#include "DMA.h"
#include "DMAplus.h"
#include "MACD.h"
#include "RSI.h"
#include "LinearRegNorm.h"
#include <iomanip>
#include <cmath>
class Best{
public:
    string symbolName;
    chrono::year_month_day startDate;
    chrono::year_month_day endDate;
    chrono::year_month_day modStartDate;

    chrono::steady_clock::time_point adxStart,adxEnd;
    chrono::steady_clock::time_point basicStart,basicEnd;
    chrono::steady_clock::time_point dmaStart,dmaEnd;
    chrono::steady_clock::time_point dmaPlusStart,dmaPlusEnd;
    chrono::steady_clock::time_point macdStart,macdEnd;
    chrono::steady_clock::time_point rsiStart,rsiEnd;
    chrono::steady_clock::time_point regStart,regEnd;


    PriceTable table;
    ADXStrat adx;
    Basic basic;
    DMA dma;
    DMAPlus dmaPlus;
    MACDStrat macd;
    RSI rsi;
    LinearRegression regression;
    Best(chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName);
    Best();
    void main();

    void adxHelper();
    void basicHelper();
    void dmaHelper();
    void dmaPlusHelper();
    void macdHelper();
    void rsiHelper();
    void regHelper();
};