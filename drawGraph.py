import pandas as pd 
import matplotlib.pyplot as plt
dfPrice = pd.read_csv("./bin/stockCSV/SBIN.csv")
adx_threshold=20

oversold_threshold=30
overbough_threshold=70

dfPrice = dfPrice[["DATE","CLOSE"]]
dfFltData = pd.read_csv("./bin/stockCSV/RSI.csv")
fig,ax1 = plt.subplots()
dfPrice["DATE"] = pd.to_datetime(dfPrice["DATE"])
dfFltData["Date"] = pd.to_datetime(dfFltData["Date"])
ax1.plot(dfPrice["DATE"],dfPrice["CLOSE"],label="CLOSE",color="green")
ax2 = ax1.twinx()
ax2.plot(dfFltData["Date"],dfFltData["RSI"],label="RSI",color="orange")
#plt.legend()
#ax2.axhline(y=adx_threshold,color='r',linestyle='-')
ax2.axhline(y=oversold_threshold,color='r',linestyle='-')
ax2.axhline(y=overbough_threshold,color='r',linestyle='-')
ax1.legend(loc = 0)
ax2.legend(loc = 1)
plt.show()
