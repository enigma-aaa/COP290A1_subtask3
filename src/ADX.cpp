#include "ADX.h"
#include "CommonCons.h"
ADXStrat::ADXStrat(int n,int x,double adx_threshold,chrono::year_month_day startDate,chrono::year_month_day endDate,string symbolName):
n(n),x(x),adx_threshold(adx_threshold),startDate(startDate),endDate(endDate),symbolName(symbolName){
    noShares = 0;
    curBal = 0;
    modStartDate = subtractDate(startDate,max(2*n,30));
    alphaATR = 2.0/((double)(n+1));
    table = nullptr;
}
ADXStrat::ADXStrat(){}
void ADXStrat::buy(){
    noShares++;
    curBal = curBal - curPrice;
    stats.addRow(curDate,"BUY",1,curPrice);
}
void ADXStrat::sell(){
    noShares--;
    curBal = curBal + curPrice;
    stats.addRow(curDate,"SELL",1,curPrice);
} 
void ADXStrat::first(int startDateLoc){
    int startDate_n_Loc = startDateLoc -n;
    PriceTableRow& curRow  = table->rows[startDateLoc];
//    PriceTableRow& prevRow = table->rows[startDateLoc-1];
    double a = curRow.high - curRow.low;
    double b = curRow.high - curRow.prevClose;
    double c = curRow.low - curRow.prevClose;
    curTR = max(a,b,c);
    ATR = curTR;
    DIplus = 0;
    DIminus = 0;
}
void ADXStrat::writeCashFlow(chrono::year_month_day curDate){
    flow.addRow(curDate,curBal);
}

void ADXStrat::writeFinalPNL(){
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
void ADXStrat::check(){
    if(ADX > adx_threshold){
        if(noShares < x){
            buy();
        }
    } 
    if(ADX < adx_threshold){
        if(noShares > -x){
            sell();
        }
    }
}
void ADXStrat::squareOff(){
    curBal = curBal + noShares * curPrice;
    noShares = 0;
}
void ADXStrat::writeCSVfiles()
{
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats = baseFilePath + ORDER_STATS;
    flow.writeToCsv(csv_cashflow);
    stats.writeToCsv(csv_order_stats);        
}

double ADXStrat::max(double a,double b){
    if(a > b){
        return a;
    }
    return b;
}
double ADXStrat::max(double a,double b,double c){
    return max(max(a,b),c);
}
void ADXStrat::main(){
    PriceTable createTable = getPriceTable(symbolName,modStartDate,endDate);
    multiMain(&createTable);
    writeCSVfiles();
    writeFinalPNL();
}
void ADXStrat::multiMain(PriceTable* srcTable){
    //table = getPriceTable(symbolName,modStartDate,endDate);
    table = srcTable;
    curPrice = 0;
    int startDateLoc = table->rows.size();
    for(int i=0;i<table->rows.size();i++){
        if(grtrEqual(table->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }
    first(startDateLoc);
    writeCashFlow(table->rows[startDateLoc].date);
    //may have to change starting index to deal with threshold=0
    for(int i=startDateLoc+1;i<table->rows.size();i++){
        curPrice = table->rows[i].close;
        curDate = table->rows[i].date;
        PriceTableRow& curRow = table->rows[i];
        PriceTableRow& prevRow = table->rows[i-1];
        curTR = max(max(curRow.high - curRow.low,curRow.high - curRow.prevClose),curRow.low - curRow.prevClose);
        DMplus = max(0.0,curRow.high - prevRow.high);
        DMminus = max(0.0,curRow.low - prevRow.low);
        ATR = alphaATR*curTR - alphaATR*ATR + ATR;
        // ATR = alphaATR*curTR - alphaATR * ATR + ATR;
        if(ATR != 0){
            double DMplusByATR = DMplus/ATR;
            double DMminusByATR = DMminus/ATR;
            // DIplus = alphaATR*(DIplus - DMplusByATR) + DMplusByATR;
            DIplus = alphaATR*DMplusByATR - alphaATR*DIplus + DIplus;
            // DIplus = alphaATR*DIplus - alphaATR* DMplusByATR + DMplusByATR;
            // DIminus = alphaATR*(DIminus - DMminusByATR) + DMminusByATR;
            DIminus = alphaATR*DMminusByATR - alphaATR*DIminus + DIminus;
            // DIminus = alphaATR*DIminus - alphaATR *  DMminusByATR + DMminusByATR;
            if((DIminus+DIplus) != 0){
                DX = (((DIplus - DIminus)*100.0)/(DIplus + DIminus));
                //cout << " DIplus+DIminus:" << (DIplus+DIminus);
                ADX = alphaATR*DX - alphaATR*ADX + ADX;     
                // ADX = alphaATR*DX - alphaATR *  ADX + ADX;     
                //cout << " ADX is:" << ADX << endl;
                cout<<ADX<<" "<<ATR<<" "<<DX<<" "<<DIplus<<" "<<DIminus<< endl;
                check();
            }else{
                ADX = 0;
            }
        }else{
            ATR= 0;
            ADX = 0;
        }
        writeCashFlow(curRow.date);
    }
    //writeCSVfiles();
    squareOff();
    //writeFinalPNL();
}