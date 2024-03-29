from jugaad_data.nse import stock_df
from datetime import date
import pandas as pd
import sys

n = len(sys.argv)
symbolName = sys.argv[1]

startYear = int(sys.argv[2])
startMonth = int(sys.argv[3])
startDay = int(sys.argv[4])
#print("Python start")
#print("startYear:",startYear,"startMonth:",startMonth,"startDay:",startDay)
startDate = date(startYear,startMonth,startDay)

endYear = int(sys.argv[5])
endMonth = int(sys.argv[6])
endDay = int(sys.argv[7])
#print("endYear:",endYear,"endMonth:",endMonth,"endDay:",endDay)
endDate = date(endYear,endMonth,endDay)

csvFileName = sys.argv[8]
#try:
#    createFile = open(csvFileName,"x")
#except FileExistsError:
#    pass
df = stock_df(symbol=symbolName,from_date=startDate,to_date=endDate,series="EQ")
df = df[["DATE","OPEN","HIGH","LOW","PREV. CLOSE","CLOSE","VWAP","NO OF TRADES"]]
df = df.drop_duplicates("DATE")
df.set_index("DATE")
df[::-1].to_csv(csvFileName,index=False)
#print("number of rows of dataframe:",len(df))
#print("Python end")