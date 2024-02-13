#include "dateUtil.h"
#include "CommonCons.h"
#include <vector>

PriceTable getPriceTable(string& symbolName,chrono::year_month_day startDate,chrono::year_month_day endDate){
    string command = "python3 ./src/stockData.py ";
    command += symbolName + " ";
    int startYear,startMonth,startDay;
    startYear = int(startDate.year());
    startMonth = unsigned(startDate.month());
    startDay = unsigned(startDate.day());
    
    //cout << "startYear:" << startYear << " " << "startMonth:" << startMonth << " startDay:" << startDay << endl;

    int endYear,endMonth,endDay;
    endYear = int(endDate.year());
    endMonth = unsigned(endDate.month());
    endDay = unsigned(endDate.day());
    string filePath = CSV_FILE_PATH + symbolName + ".csv";
    
    //cout << "endYear:" << endYear << " " << "endMonth:" << endMonth << " endDay:" << endDay << endl;
    command += to_string(startYear) + " " + to_string(startMonth) + " " + to_string(startDay) + " ";
    command += to_string(endYear) + " " + to_string(endMonth) + " " + to_string(endDay) + " ";
    command += filePath + " ";

    cout << "command is:" << command << endl;
    system(command.c_str());

    PriceTable table;
    table.read_csv(filePath);
    /*for(PriceTableRow& row:table.rows){
        cout << row.getString() << endl;
    } */ 
    //cout << "Number of rows in table:" << table.rows.size() << endl;
    return table;  
}
/* a-b */
int difference(chrono::year_month_day a,chrono::year_month_day b){
    chrono::time_point aTimePoint = chrono::sys_days{a};
    chrono::time_point bTimePoint = chrono::sys_days{b};
    time_t aTime = chrono::system_clock::to_time_t(aTimePoint);
    time_t bTime = chrono::system_clock::to_time_t(bTimePoint);
    time_t diff = aTime - bTime;
    time_t oneDay = 60*60*24;
    int noDays = diff/oneDay;
    return noDays;
}

chrono::year_month_day subtractDate(chrono::year_month_day date,int n){
    chrono::time_point time = chrono::sys_days{date};
    time_t epochTime = chrono::system_clock::to_time_t(time);
    time_t oneDay = 60*60*24;
    epochTime = epochTime - oneDay*n;
    tm local_tm = *localtime(&epochTime);
    //cout << "Years is" << local_tm.tm_year + 1900 << endl;
    //cout << "Month is:" << local_tm.tm_mon + 1 << endl;
    //cout << "Day is:" << local_tm.tm_mday << endl;
    //relative to 1900 hence have to add here
    int year = local_tm.tm_year + 1900;
    //months starting from 0
    int month = local_tm.tm_mon + 1;
    int day = local_tm.tm_mday;
    return chrono::year_month_day(chrono::year(year),chrono::month(month),chrono::day(day));
}

void printDate(chrono::year_month_day date){
    int year = int(date.year());
    int month = unsigned(date.month());
    int day = unsigned(date.day());
    cout << year << "/" << month << "/" << day;
}

chrono::year_month_day min(chrono::year_month_day date1,chrono::year_month_day date2){
    chrono::time_point timePoint1 = chrono::sys_days{date1};
    chrono::time_point timePoint2 = chrono::sys_days{date2};
    time_t time1 = chrono::system_clock::to_time_t(timePoint1);
    time_t time2 = chrono::system_clock::to_time_t(timePoint2);
    if(time1 < time2){
        return date1;
    }
    return date2;
}
//Assum
chrono::year_month_day min(std::vector<chrono::year_month_day> list){
    if(list.size() == 1){return list[0];}
    chrono::year_month_day result = min(list[0],list[1]);
    for(int i=0;i<list.size();i++){
        result = min(result,list[i]);
    }
    return result;
}

bool grtrEqual(chrono::year_month_day a,chrono::year_month_day b){
    chrono::time_point timePointA = chrono::sys_days{a};
    chrono::time_point timePointB = chrono::sys_days{b};
    time_t timeA = chrono::system_clock::to_time_t(timePointA);
    time_t timeB = chrono::system_clock::to_time_t(timePointB);
    return timeA >= timeB;    
}