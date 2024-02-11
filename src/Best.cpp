#include <omp.h>
#include "Best.h"
Best::Best(chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName)
:symbolName(symbolName),startDate(startDate),endDate(endDate){
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
    adx = ADXStrat(nOther,x,adx_threshold,startDate,endDate,symbolName);
    basic = Basic(nBasic,x,startDate,endDate,symbolName);
    dma = DMA(nDMA,x,pDMA,startDate,endDate,symbolName);
    dmaPlus = DMAPlus(nOther,x,pDMAplus,max_hold_days,c1,c2,startDate,endDate,symbolName);
    macd = MACDStrat(x,startDate,endDate,symbolName);
    rsi = RSI(nOther,x,oversold_threshold,overbought_threshold,startDate,endDate,symbolName);
    modStartDate = min({adx.modStartDate,basic.modStartDate,dma.modStartDate,dmaPlus.modStartDate,macd.modStartDate,rsi.modStartDate});
    table = getPriceTable(symbolName,modStartDate,endDate);
}
Best::Best(){}

void Best::main(){
    chrono::steady_clock::time_point begin = chrono::steady_clock::now(); 
    {
        #pragma omp parallel
        {
            #pragma omp single
            {
            #pragma omp task
                adx.multiMain(&table);
                cout << "adx bal is:" << adx.curBal << endl;
            #pragma omp task
                basic.multiMain(&table);
                cout << "basic bal is:" << basic.curBal << endl;
            #pragma omp task
                dma.multiMain(&table);
                cout << "dma bal is:" << dma.curBal << endl;
            #pragma omp task
                dmaPlus.multiMain(&table);
                cout << "dmaPlus bal is:" << dmaPlus.curBal << endl;
            #pragma omp task
                macd.multiMain(&table);
                cout << "macd bal is:" << macd.curBal << endl;
            #pragma omp task
                rsi.multiMain(&table);
                cout << "rsi bal is:" << rsi.curBal << endl;
            }
        }
    }
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
    cout << "timeNano:" << timeNano << "ns" << endl;
}