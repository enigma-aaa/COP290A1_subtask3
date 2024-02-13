#include "DMAplus.h"
#include "CommonCons.h"
#include "CommonCons.h"

DMAPlus::DMAPlus(int n,int x,double p,int max_hold_days,
double c1,double c2,chrono::year_month_day startDate,
chrono::year_month_day endDate,string symbolName)
:n(n),x(x),p(p),max_hold_days(max_hold_days),c1(c1),c2(c2),symbolName(symbolName),startDate(startDate),endDate(endDate){
    //cout << "constructed object with" << endl;
    //cout << "n is:" << n << " x is:" << x << " p is:" << p << " max_hold_days:" << max_hold_days << " c1:" << c1 << " c2:" << c2 << endl;
    modStartDate = subtractDate(startDate,max(2*n,30));
    table = nullptr;
}
DMAPlus::DMAPlus() {}
//first+n is the index of the start function
void DMAPlus::firstPrice(int first){
    curAbsoluteSum = 0;
    curBal = 0;
    curLoc = first + n;
    for(int i=first+1;i<n+first+1;i++){            
        double curDiff = abs(table->rows[i].close - table->rows[i-1].close);
        curAbsoluteSum += curDiff;    
    }
    //priceChange_n = table->rows[first+n] - table->rows[first];
    //not needed here handeled in the loop
    smoothingFactor = 0.5;
    AMA = table->rows[first+n].close;
    curPrice = AMA;
    noShares = 0;
    //cannot call check here as check also updates the values involved probably 
    //not needed here either as difference between AMA and price is zero
    int shareDelta =  check();
    if(shareDelta > 0){
        stats.addRow(table->rows[first+n].date,"BUY",shareDelta,AMA);
    }else if(shareDelta < 0){
        stats.addRow(table->rows[first+n].date,"SELL",-shareDelta,AMA);
    }
}
void DMAPlus::buy()
{
    noShares++;
    curBal = curBal - curPrice;
    //forgot to add quantity column have to handle that
    //flow.addRow(curDate,curBal);
    if(!sellDates.empty()){
        //if have sold some stock as in shorted it but have not balanced it out then do this
        //sellDates.front();
        sellDates.pop();
    }else{
        buyDates.push(curLoc);
    }
}
void DMAPlus::sell()
{
    noShares--;
    curBal = curBal + curPrice;
    //flow.addRow(curDate,curBal);
    if(!buyDates.empty()){
        buyDates.pop();
    }else{
        sellDates.push(curLoc);
    }
}
int DMAPlus::handleMaxHold(){
    if(!buyDates.empty()){
        int oldestBuy = buyDates.front();
        int dayDiff = curLoc-oldestBuy;
        //no need to check later elements in queue as only one of them can be oldest
        /*Ask sir about whether equal to required here*/
        if(dayDiff >= max_hold_days){
            noShares--;
            curBal = curBal + curPrice;
            buyDates.pop();
            //reversing buy so is a sell
            return -1;
        }
    }
    if(!sellDates.empty()){
        int oldestSell = sellDates.front();
        int dayDiff = curLoc - oldestSell;
        /*Ask sir about whether equal to required here*/
        if(dayDiff >= max_hold_days){
            noShares++;
            curBal = curBal - curPrice;
            sellDates.pop();
            //reversing sell so is a buy
            return 1;
        }
    }
    return 0;
}
int DMAPlus::check(){
    //cofirm p given here as pre multiplied as 100
    //assuming p is non zero
    // cout<<AMA<<endl;cout
    if(curPrice >= (AMA + ((AMA*p)/100))) 
    {
        if(noShares < x)
        {
            buy() ;
            return 1 ;
        }
        else if(noShares == x)
        {
            return 2 ;
        }
    }
    if(curPrice <= (AMA - ((AMA * p) /100)))
    {
        if(noShares > -x)
        {
            sell() ;
            return -1 ;
        }
        else if(noShares == -x)
        {
            return -2 ;
        }
    }
    return 0;
}
void DMAPlus::writeCashFlow(){
    flow.addRow(curDate,curBal);
}
void DMAPlus::writeCSVfiles(){
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats = baseFilePath + ORDER_STATS;
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);  
}
void DMAPlus::writeFinalPNL(){
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
void DMAPlus::main(){
    PriceTable createTable = getPriceTable(symbolName,modStartDate,endDate);
    multiMain(&createTable);
    writeCSVfiles();
    writeFinalPNL();
}
void DMAPlus::multiMain(PriceTable* srcTable){
    table = srcTable;
    int tableSize = table->rows.size();
    int startDateLoc = -1;
    for(int i=0;i<table->rows.size();i++){
        if(grtrEqual(table->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }

    int startDate_n_Loc = startDateLoc - n;
    //add a check to first price
    firstPrice(startDate_n_Loc);
    curDate = table->rows[startDateLoc].date;
    writeCashFlow();
    //cout << "Initial AMA is:" << AMA <<" smoothing:factor" << smoothingFactor<< endl;
    for(int i=startDateLoc+1;i<table->rows.size();i++){
        curLoc = i;
        curPrice = table->rows[i].close;
        priceChange_n = table->rows[i].close - table->rows[i-n].close;
        curDate = table->rows[i].date;
        //definetly have to crosscheck these indices
        double oldAbsDiff = abs(table->rows[i-n].close-table->rows[i-n-1].close);
        double newAbsDiff = abs(table->rows[i].close-table->rows[i-1].close);
        curAbsoluteSum = curAbsoluteSum - oldAbsDiff + newAbsDiff;
        ER = priceChange_n/curAbsoluteSum;
        if(curAbsoluteSum == 0){
            cerr << "absolute sum was zero Error:" << endl;
        }
        double factorNum = (2*ER)/(1+c2) - 1;
        double factorDenom = (2*ER)/(1+c2) + 1;
        double factor = factorNum/factorDenom;
        smoothingFactor = smoothingFactor + c1 * (factor - smoothingFactor);
        double temp = AMA + smoothingFactor*(curPrice - AMA);
        //cout<<"temp : "<<temp<<endl;
        AMA = temp ;
        int shareDelta1 =  check();
        int shareDelta2 = handleMaxHold();
        int shareDelta = shareDelta1 + shareDelta2;
        if(shareDelta1 == 2 && shareDelta2 == -1)
        {
            buy() ;
            shareDelta = 0 ;
        }
        else if(shareDelta1  == -2 && shareDelta2 == 1)
        {
            sell() ;
            shareDelta = 0 ;
        }
        else if(shareDelta1 == 2)
        {
            shareDelta = 0 ;
        }
        else if(shareDelta1 == -2)
        {
            shareDelta = 0;
        }
        if(shareDelta > 0){
            stats.addRow(curDate,"BUY",shareDelta,curPrice);
        }else if(shareDelta < 0){
            stats.addRow(curDate,"SELL",-shareDelta,curPrice);
        }
        writeCashFlow();
        // cout << "AMA: " << AMA << " curDate "; printDate(curDate);
        // cout << " noShares: " << noShares << "smoothing factor: " << smoothingFactor << " curPrice: " << curPrice << " factor: " << factor << endl;
        // cout << "i is:" << i << " curBal is:" << curBal  << endl;
    }
    squareOff();
}
void DMAPlus::squareOff(){
    curBal = curBal + noShares * table->rows.back().close;
    noShares = 0;
    //if correctly understood only have to clear this
    while(!sellDates.empty()){ sellDates.pop(); }
    while(!buyDates.empty()){ buyDates.pop(); }
}