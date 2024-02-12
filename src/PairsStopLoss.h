#pragma once 
#include "parser.h"
#include "dateUtil.h"
#include <cmath>
#include <iomanip>

class Node{
    public : 
    Node* next ;
    Node* prev ;
    int buy_sell_location ;
    double condition1 ;
    double condition2 ;
    Node(int i , double curMean , double curSD , double stopLossThreshold)
    {
        next = NULL ;
        prev = NULL ;
        buy_sell_location = i ;
        condition1 = stopLossThreshold*curSD + curMean ;
        condition2 = -1*stopLossThreshold*curSD + curMean ;
    }
    Node()
    {
        next = NULL ;
        prev = NULL ;
        buy_sell_location = -1 ;
        condition1 = 0 ;
        condition2 = 0 ;
    }
} ;

class LinkedList{
    public : 
    Node* head ;
    Node* tail ;
    LinkedList()
    {
        head = NULL ;
        tail = NULL ;
    }
    void removeHead()
    {
        if(head == NULL)
        {
            return  ;
        }
        else 
        head = head->next ;
        head->prev = NULL ;
    }
    void addToTail(Node* temp)
    {
        tail->next = temp ;
        temp->prev = tail ;
        tail = tail->next ;
    }
} ;
class PairsStopLoss{
public:
    int x;
    int n;
    int noShares = 0;
    double threshold;
    double stop_loss_threshold;
    int curLoc ;
    LinkedList myPortfolio ;
    string symbol1,symbol2;
    chrono::year_month_day startDate,endDate;
    chrono::year_month_day modStartDate;

    PriceTable* table1;
    PriceTable* table2;
    CashFlow flow;
    OrderStats stats1;
    OrderStats stats2;

    chrono::year_month_day curDate;
    PairsStopLoss(int x,int n,double threshold,double stop_loss_threshold,
    chrono::year_month_day startDate,chrono::year_month_day endDate,string symbol1,string symbol2);
    double curPrice1 = 0;
    double curPrice2 = 0;
    double curSpread = 0;
    double curSum = 0;
    double curMean = 0;
    double curDev = 0;
    double curSqSum = 0;
    double curBal = 0;
    double curZscore = 0;

    void buy();
    void sell();
    void first(int startDateLoc);
    void writeCashFlow(chrono::year_month_day curDate);
    void writeFinalPNL();
    int check();
    int checkForThresholds() ;
    void squareOff();
    void writeCSVfiles();
    void main();
};