#pragma once
#include <vector>
#include <chrono>
#include <iostream>
#include <ctime>
#include "parser.h"
#define CSV_FILE_PATH "./bin/stockCSV/"
using namespace std;

PriceTable getPriceTable(string& symbolName,chrono::year_month_day startDate,chrono::year_month_day endDate);
/* a-b */
int difference(chrono::year_month_day a,chrono::year_month_day b);

chrono::year_month_day subtractDate(chrono::year_month_day date,int n);

void printDate(chrono::year_month_day date);

//returns true if date a greater than or equal to date b
bool grtrEqual(chrono::year_month_day a,chrono::year_month_day b);

chrono::year_month_day min(chrono::year_month_day date1,chrono::year_month_day date2);
chrono::year_month_day min(vector<chrono::year_month_day> list);