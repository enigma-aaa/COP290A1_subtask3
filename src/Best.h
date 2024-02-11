#pragma once
#include "parser.h"
#include "dateUtil.h"
#include "ADX.h"
#include "basicStart.h"
#include "DMA.h"
#include "DMAplus.h"
#include "MACD.h"
#include "RSI.h"
#include <iomanip>
#include <cmath>
class Best{
public:
    string symbolName;
    chrono::year_month_day startDate;
    chrono::year_month_day endDate;
    chrono::year_month_day modStartDate;
    PriceTable table;
    ADXStrat adx;
    Basic basic;
    DMA dma;
    DMAPlus dmaPlus;
    MACDStrat macd;
    RSI rsi;
    Best(chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName);
    Best();
    void main();
};