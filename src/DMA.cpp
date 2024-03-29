#include "DMA.h"
#include "CommonCons.h"

DMA::DMA(int n, int x, double p, chrono::year_month_day startDate, chrono::year_month_day endDate, string symbolName):
n(n),x(x),p(p),startDate(startDate),endDate(endDate),symbolName(symbolName)
{
    modStartDate = subtractDate(startDate, max(2 * n,30));
    noShares = 0 ;
    table = nullptr;
}
DMA::DMA() {}
void DMA::buy(chrono :: year_month_day date)
{
    noShares++ ;
    curBal = curBal - curPrice ;
    stats.addRow(date,"BUY",1,curPrice) ;
}
void DMA::sell(chrono :: year_month_day date)
{
    noShares-- ;
    curBal = curBal + curPrice ;
    stats.addRow(date,"SELL" ,1 , curPrice) ;
}
void DMA::writeCashFlow(chrono::year_month_day curDate){
    flow.addRow(curDate,curBal);
}
void DMA::check()
{
    curSum = curSum -table->rows[curLoc-n].close + curPrice ;
    
    cursquaredSum = cursquaredSum - (table->rows[curLoc-n].close)*(table->rows[curLoc-n].close) + curPrice * curPrice ;
    double sd = sqrt(cursquaredSum/n - ((curSum * curSum)/(n*n))) ;
    double curMean = curSum/n ;

    /*Both buy and sell may occur here because of greater than or equal to*/
    double deviation = (curPrice - curMean)/sd;
    if(curPrice >= curMean + p*sd)
    {
        //buy
        if(noShares<x)
        {
            buy(table->rows[curLoc].date) ;
        }
    }
    else if(curPrice <= curMean-p*sd)
    {
        //sell
        if(noShares > -x )
        {
            sell(table->rows[curLoc].date) ;
        }
        
    }
    /*Included -(n-1) instead of -n here becuase sir said we have to include current day and seemed to say total n days*/

}

void DMA::squareOff()
{
    curBal  = curBal + noShares * table->rows.back().close ; 
    noShares = 0 ;
}
void DMA::writeCSVfiles()
{
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats = baseFilePath + ORDER_STATS;
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);        
}
void DMA::writeFinalPNL(){
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
void DMA::main()
{
    PriceTable createTable = getPriceTable(symbolName, modStartDate, endDate);
    multiMain(&createTable);
    writeCSVfiles();
    writeFinalPNL();
}
void DMA::multiMain(PriceTable* srcTable)
{
    curBal = 0;
    table = srcTable;
    int tableSize = table->rows.size();
    int startDateLoc = table->rows.size();
    curPrice = 0 ;
    for (int i = 0; i < table->rows.size(); i++)
    {
        if(grtrEqual(table->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }
    /*Have to ask sir since current day has to be included might have to do n-1 here*/
    int startDate_n_Loc = startDateLoc - n;
    curSum = 0 ;
    cursquaredSum = 0;
    for(int i = startDate_n_Loc ; i<startDate_n_Loc+n ; i++)
    {
        curSum +=  table->rows[i].close ;
        cursquaredSum += table->rows[i].close * table->rows[i].close;
    }   

    for(int i = startDateLoc ; i<  table->rows.size() ; i++)
    {
        curPrice = table->rows[i].close ;
        curLoc = i;
        check() ;
        writeCashFlow(table->rows[i].date);
    }
    squareOff() ;
    //writeCSVfiles();
    //writeFinalPNL();
}
