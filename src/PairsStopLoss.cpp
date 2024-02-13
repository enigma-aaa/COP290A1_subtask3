#include "PairsStopLoss.h"
#include "CommonCons.h"
#include <queue>
PairsStopLoss::PairsStopLoss(int x,int n,double threshold,double stop_loss_threshold,
chrono::year_month_day startDate,
chrono::year_month_day endDate,string symbol1,
string symbol2):x(x),n(n),threshold(threshold),stop_loss_threshold(stop_loss_threshold) ,  startDate(startDate),
endDate(endDate),symbol1(symbol1),symbol2(symbol2){
    modStartDate = subtractDate(startDate,2*n);
    cout << "called pairs constructor" << endl;
    noShares = 0 ;
    cout<<"stop "<<stop_loss_threshold<<endl;
}

void PairsStopLoss::buy(){
    cout<<"inside buying "<<shortSellStocks.size()<<" "<<boughtStocks.size()<<endl;
    if(shortSellStocks.size()!=0)
    {
        shortSellStocks.erase(shortSellStocks.begin()) ;
    }
    else
    {
        Node temp(curLoc , curMean , curDev , stop_loss_threshold) ;
        boughtStocks.insert(temp) ;
        cout<<"conditions "<<temp.condition1<<" "<<temp.condition2<<" "<<curMean<<" "<<curDev<<" "<<stop_loss_threshold<<endl;
        cout<<"Pushed into boughtStocks "<<boughtStocks.size()<<endl;
    }
    curBal = curBal -curPrice1 + curPrice2 ;
    noShares++ ;
    // if(noShares < 0)
    // {
    //     noShares++ ;
    //     curBal = curBal - curPrice1 + curPrice2 ;
    //     myPortfolio.removeHead() ;
    // }
    // else
    // {
    //     noShares++;
    //     curBal = curBal - curPrice1 + curPrice2;
    //     Node* temp = new Node(curLoc , curMean , curDev , stop_loss_threshold) ;
    //     myPortfolio.addToTail(temp) ;
    // }
}

void PairsStopLoss::sell(){
    cout<<"inside buying "<<shortSellStocks.size()<<" "<<boughtStocks.size()<<endl;
    if(boughtStocks.size()!=0)
    {
        boughtStocks.erase(boughtStocks.begin()) ; 
    }
    else
    {
        Node temp(curLoc , curMean ,curDev , stop_loss_threshold) ;
        shortSellStocks.insert(temp);
        
        cout<<"conditions "<<temp.condition1<<" "<<temp.condition2<<" "<<curMean<<" "<<curDev<<" "<<stop_loss_threshold<<endl;
        cout<<"Pushed into shortSellStocks "<<shortSellStocks.size()<<endl;
    }
    curBal = curBal + curPrice1 - curPrice2 ;
    noShares-- ;
    // if(noShares > 0)
    // {
    //     noShares--;
    //     curBal = curBal + curPrice1 - curPrice2;
    //     myPortfolio.removeHead() ;
    // }
    // else
    // {
    //     noShares-- ;
    //     curBal = curBal +curPrice1 - curPrice2 ;
    //     Node* temp = new Node(curLoc, curMean , curDev , stop_loss_threshold) ;
    //     myPortfolio.addToTail(temp) ;
    // }
}

void PairsStopLoss::first(int startDateLoc){        
    int startDate_n_Loc = startDateLoc - n;
    curLoc = startDateLoc ;
    for(int i=startDate_n_Loc;i<startDateLoc;i++){
        curPrice1 = table1->rows[i].close;
        curPrice2 = table2->rows[i].close;
        curSum += (curPrice1 - curPrice2);                
        curSqSum += (curPrice1-curPrice2)*(curPrice1-curPrice2);
    }
}

void PairsStopLoss::writeCashFlow(chrono::year_month_day curDate){
    flow.addRow(curDate,curBal);
}
void PairsStopLoss::writeFinalPNL(){
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
int PairsStopLoss::check(){
    curMean = curSum/n;
    curDev = curSqSum/n - curMean*curMean;   
    curDev = sqrt(curDev); 
    curZscore = (curSpread - curMean)/curDev;
    cout << "zScore is:" << curZscore << endl;
    if(curZscore > threshold){
        if(noShares > -x){
            sell() ;

            return -1 ;       
        }
    }
    if(curZscore < -threshold){
        if(noShares < x){
            buy() ;

            return 1 ;
        }
    }    
    return 0 ;
}

int PairsStopLoss:: checkForThresholds()
{
    // Node* temp = myPortfolio.head ;
    int stocksCrossed = 0 ;

    for(auto it = boughtStocks.begin() ; it!= boughtStocks.end();)
    {
        if(curSpread > (*it).condition1 )
        {
            curBal = curBal - curPrice1 + curPrice2 ;
            stocksCrossed++ ;
            noShares++ ;
            it = boughtStocks.erase(it) ;
        }
        else if(curSpread < (*it).condition2)
        {
            curBal = curBal + curPrice1 - curPrice2 ;
            stocksCrossed-- ;
            noShares-- ;
            it = boughtStocks.erase(it) ;
        }
        else
        {
            it++ ;
        }
    }
    for(auto it = shortSellStocks.begin() ; it!= shortSellStocks.end();)
    {
        if(curSpread > (*it).condition1 )
        {
            cout<<"removing "<<endl;
            curBal = curBal - curPrice1 + curPrice2 ;
            stocksCrossed++ ;
            noShares++ ;
            it = shortSellStocks.erase(it) ;
        }
        else if(curSpread < (*it).condition2)
        {
            cout<<"removing "<<endl;
            curBal = curBal + curPrice1 - curPrice2 ;
            stocksCrossed-- ;
            noShares-- ;
            it = shortSellStocks.erase(it) ;
        }
        else
        {
            it++ ;
        }
    }
    // while(temp != NULL)
    // {
    //     if(curSpread >= temp->condition1)
    //     {
    //         stocksCrossed++;
    //         curBal = curBal -curPrice1 + curPrice2 ;
    //         if(temp->prev == NULL)
    //         {
    //             myPortfolio.head = temp->next ;
    //             temp = myPortfolio.head ;
    //         }
    //         else
    //         {
    //             temp->prev->next = temp->next ;
    //             temp = temp->next ;
    //         }
    //         noShares++ ;

    //     }
    //     else if(curSpread <= temp->condition2)
    //     {
    //         stocksCrossed--;
    //         noShares-- ;
    //         curBal = curBal + curPrice1 - curPrice2 ;
    //         if(temp->prev == NULL)
    //         {
    //             myPortfolio.head = temp->next ;
    //             temp = myPortfolio.head ;
    //         }
    //         else
    //         {
    //             temp->prev->next = temp->next ;
    //             temp->next->prev = temp->prev ;
    //             temp = temp->next ;
            
    //         }
    //     }
    //     else
    //     {
    //         temp = temp->next; 
    //     }
    // }
    return stocksCrossed ;
}

void PairsStopLoss::squareOff(){
    curBal = curBal + noShares*(curPrice1-curPrice2);
}

void PairsStopLoss::writeCSVfiles()
{
    string baseFilePath = BASE_FILE_PATH;
    string csv_cashflow = baseFilePath + CASHFLOW;
    string csv_order_stats1 = baseFilePath + "order_statistics_1.csv";
    string csv_order_stats2 = baseFilePath + "order_statistics_2.csv";
    flow.writeToCsv(csv_cashflow);
    stats1.writeToCsv(csv_order_stats1);
    stats2.writeToCsv(csv_order_stats2);        
}

void PairsStopLoss::main(){
    PriceTable createTable1 = getPriceTable(symbol1,modStartDate,endDate);        
    PriceTable createTable2 = getPriceTable(symbol2,modStartDate,endDate);
    table1 = &createTable1;
    table2 = &createTable2;
    curPrice1 = 0;
    curPrice2 = 0;
    int startDateLoc = -1;
    for(int i=0;i<table1->rows.size();i++){
        if(grtrEqual(table1->rows[i].date,startDate)){
            startDateLoc = i;
            break;
        }
    }
    if(startDateLoc == -1){
        cout << "start date not located in table for some reason" << endl;
    }
    first(startDateLoc);
    //for handle stop loss threshold instead of just queue might have to keep a linked list here
    for(int i=startDateLoc;i<table1->rows.size();i++){
        curLoc = i ;
        curPrice1 = table1->rows[i].close;
        curPrice2 = table2->rows[i].close;
        curSpread = curPrice1 - curPrice2 ;
        //assuming current rows same in both
        curDate = table1->rows[i].date;
        double oldPrice1 = table1->rows[i-n].close;
        double oldPrice2 = table2->rows[i-n].close;
        curSum = curSum - (oldPrice1 - oldPrice2) + (curPrice1 - curPrice2);
        curSqSum = curSqSum - (oldPrice1 - oldPrice2)*(oldPrice1-oldPrice2) + (curPrice1 - curPrice2)*(curPrice1-curPrice2);
        int strategyResultToday = check();
        int noCrossingThresholds = checkForThresholds();
        int totalChangeinStocks = strategyResultToday + noCrossingThresholds ;
        if(totalChangeinStocks > 0)
        {
            stats1.addRow(curDate,"BUY",totalChangeinStocks,curPrice1);         
            stats2.addRow(curDate,"SELL",totalChangeinStocks,curPrice1);         
        }
        else if(totalChangeinStocks < 0)
        {
            stats1.addRow(curDate,"SELL",-totalChangeinStocks,curPrice1);         
            stats2.addRow(curDate,"BUY",-totalChangeinStocks,curPrice1);         
        }
        writeCashFlow(curDate);
    }
    writeCSVfiles();
    squareOff();
    writeFinalPNL();
}