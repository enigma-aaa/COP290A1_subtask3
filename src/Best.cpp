#include <omp.h>
#include "Best.h"
Best::Best(chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName)
{
    this->endDate = endDate;    
    this->startDate = startDate;    
    this->symbolName = symbolName;
    int nBasic = 7;
    int nDMA = 50;
    int nOther = 14;
    int x = 5;
    int adx_threshold = 25;
    int oversold_threshold = 30;
    int overbought_threshold = 70;
    int max_hold_days = 28;
    double c1 = 2;
    double c2 = 0.2;
    double pDMA = 2;
    double pDMAplus = 5;
    double pRegression = pDMA;
    adx = ADXStrat(nOther,x,adx_threshold,startDate,endDate,symbolName);
    basic = Basic(nBasic,x,startDate,endDate,symbolName);
    dma = DMA(nDMA,x,pDMA,startDate,endDate,symbolName);
    dmaPlus = DMAPlus(nOther,x,pDMAplus,max_hold_days,c1,c2,startDate,endDate,symbolName);
    macd = MACDStrat(x,startDate,endDate,symbolName);
    rsi = RSI(nOther,x,oversold_threshold,overbought_threshold,startDate,endDate,symbolName);
    chrono::year_month_day train_start_date = subtractDate(startDate,365);
    chrono::year_month_day train_end_date = subtractDate(endDate,365);
    //what is p for linear regression
    regression = LinearRegression(train_start_date,train_end_date,startDate
                                ,endDate,pRegression,x,symbolName);
    modStartDate = min({adx.modStartDate,basic.modStartDate,dma.modStartDate,dmaPlus.modStartDate,macd.modStartDate,rsi.modStartDate});
    table = getPriceTable(symbolName,modStartDate,endDate);
}
Best::Best(){}

void Best::adxHelper(){
    adxStart = chrono::steady_clock::now();
    adx.multiMain(&table);
    adxEnd = chrono::steady_clock::now();
}
void Best::basicHelper(){
    basicStart = chrono::steady_clock::now();
    basic.multiMain(&table);
    basicEnd = chrono::steady_clock::now();
}
void Best::dmaHelper(){
    dmaStart = chrono::steady_clock::now();
    dma.multiMain(&table);
    dmaEnd = chrono::steady_clock::now();
}
void Best::dmaPlusHelper(){
    dmaPlusStart = chrono::steady_clock::now();
    dmaPlus.multiMain(&table);
    dmaPlusEnd = chrono::steady_clock::now();
}
void Best::macdHelper(){
    macdStart = chrono::steady_clock::now();
    macd.multiMain(&table);
    macdEnd = chrono::steady_clock::now();
}
void Best::rsiHelper(){
    rsiStart = chrono::steady_clock::now();
    rsi.multiMain(&table);
    rsiEnd = chrono::steady_clock::now();
}
void Best::regHelper(){
    regStart = chrono::steady_clock::now();
    regression.fit();
    regression.multiMain(&table);
    regEnd = chrono::steady_clock::now();
}

void Best::main(){
    chrono::steady_clock::time_point begin = chrono::steady_clock::now(); 
    {
    #pragma omp parallel
        {
    #pragma omp sections
            {
    #pragma omp section
                adxHelper();
    #pragma omp section
                basicHelper();
    #pragma omp section
                dmaHelper();
    #pragma omp section
                dmaPlusHelper();
    #pragma omp section
                macdHelper();
    #pragma omp section
                rsiHelper();
    #pragma omp section
                regHelper();
            }
        }
    }
    /*{
        {
            {
                adxHelper();
                basicHelper();
                dmaHelper();
                dmaPlusHelper();
                macdHelper();
                rsiHelper();
                regHelper();
            }
        }
    }*/
    cout << "adx bal is:" << adx.curBal << endl;
    cout << "basic bal is:" << basic.curBal << endl;
    cout << "dma bal is:" << dma.curBal << endl;
    cout << "dmaPlus bal is:" << dmaPlus.curBal << endl;
    cout << "macd bal is:" << macd.curBal << endl;
    cout << "rsi bal is:" << rsi.curBal << endl;
    cout << "regression bal is:" << regression.curBal << endl;

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    int minIndx = 0;
    double bestFinalBal = adx.curBal;
    if(basic.curBal > bestFinalBal){ minIndx = 1; bestFinalBal = basic.curBal;}
    if(dma.curBal > bestFinalBal){ minIndx = 2; bestFinalBal = dma.curBal;}
    if(dmaPlus.curBal > bestFinalBal){ minIndx = 3; bestFinalBal = dmaPlus.curBal;}
    if(macd.curBal > bestFinalBal){ minIndx = 4; bestFinalBal = macd.curBal;}
    if(rsi.curBal > bestFinalBal){ minIndx = 5; bestFinalBal = rsi.curBal;}
    switch(minIndx){
        case 0:
            adx.writeCSVfiles();
            adx.writeFinalPNL();
            break;
        case 1:
            basic.writeCSVfiles();
            basic.writeFinalPNL();
            break;
        case 2:
            dma.writeCSVfiles();
            dma.writeFinalPNL();
            break;
        case 3:
            dmaPlus.writeCSVfiles();
            dmaPlus.writeFinalPNL();
            break;
        case 4:
            macd.writeCSVfiles();
            macd.writeFinalPNL();
            break;
        case 5:
            rsi.writeCSVfiles();
            rsi.writeFinalPNL();
            break;
        default:
            break;
    }
    long timeNano = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();
    long adxTimeNano = chrono::duration_cast<chrono::nanoseconds>(adxEnd-adxStart).count();
    long basicTimeNano = chrono::duration_cast<chrono::nanoseconds>(basicEnd-basicStart).count();
    long dmaTimeNano = chrono::duration_cast<chrono::nanoseconds>(dmaEnd-dmaStart).count();
    long dmaPlusTimeNano = chrono::duration_cast<chrono::nanoseconds>(dmaPlusEnd-dmaPlusStart).count();
    long macdTimeNano = chrono::duration_cast<chrono::nanoseconds>(macdEnd-macdStart).count();
    long rsiTimeNano = chrono::duration_cast<chrono::nanoseconds>(rsiEnd-rsiStart).count();
    long regTimeNano = chrono::duration_cast<chrono::nanoseconds>(regEnd-regStart).count();

    cout << "timeNano:" << timeNano << "ns" << endl;
    cout << "adxTimeNano:" << adxTimeNano << "ns" << endl;
    cout << "basicTimeNano:" << basicTimeNano << "ns" << endl;
    cout << "dmaTimeNano:" << dmaTimeNano << "ns" << endl;
    cout << "dmaPlusTimeNano:" << dmaPlusTimeNano << "ns" << endl;
    cout << "macdTimeNano:" << macdTimeNano << "ns" << endl;
    cout << "rsiTimeNano:" << rsiTimeNano << "ns" << endl;
    cout << "regTimeNano:" << regTimeNano << "ns" << endl;
}