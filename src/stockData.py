from jugaad_data.nse import stock_df
from datetime import date
import pandas as pd
import sys

n = len(sys.argv)
startYear = int(sys.argv[1])
startMonth = int(sys.argv[2])
startDay = int(sys.argv[3])
startDate = date(startYear,startMonth,startDay)

endYear = int(sys.argv[4])
endMonth = int(sys.argv[5])
endDay = int(sys.argv[6])
endDate = date(endYear,endMonth,endDay)

csvFileName = sys.argv[7]
#try:
#    createFile = open(csvFileName,"x")
#except FileExistsError:
#    pass
df = stock_df(symbol="SBIN",from_date=startDate,to_date=endDate,series="EQ")
df = df[["DATE","OPEN","HIGH","LOW","PREV. CLOSE","CLOSE","VWAP","NO OF TRADES"]]
df.set_index("DATE")
df.to_csv(csvFileName,index=False)
