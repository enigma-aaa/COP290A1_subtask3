#include <chrono>
#include <iostream>
#include <ctime>
using namespace std;

chrono::year_month_day subtractDate(chrono::year_month_day date,int n){
    chrono::time_point time = chrono::sys_days{date};
    time_t epochTime = chrono::system_clock::to_time_t(time);
    time_t oneDay = 60*60*24;
    epochTime = epochTime - oneDay*n;
    tm local_tm = *localtime(&epochTime);
    //cout << "Years is" << local_tm.tm_year + 1900 << endl;
    //cout << "Month is:" << local_tm.tm_mon + 1 << endl;
    //cout << "Day is:" << local_tm.tm_mday << endl;
    int year = local_tm.tm_year;
    int month = local_tm.tm_mon;
    int day = local_tm.tm_mday;
    return chrono::year_month_day(chrono::year(year),chrono::month(month),chrono::day(day));
}