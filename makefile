Compiler=g++
Flags=-std=c++20 -c
BinFolder=./bin/
SrcFolder=./src/

all:buildProg
ifeq ($(strategy),BASIC)
	./main $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date)
endif
ifeq ($(strategy),DMA)
	./main $(strategy) $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)
endif
ifeq ($(strategy),DMA++)
	./main $(strategy) $(symbol) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) $(start_date) $(end_date)
endif
ifeq ($(strategy),MACD)
	./main $(strategy) $(symbol) $(x) $(start_date) $(end_date)
endif
ifeq ($(strategy),RSI)
	./main $(strategy) $(symbol) $(x) $(n) $(oversold_threshold) $(overbought_threshold) $(start_date) $(end_date)
endif
ifeq ($(strategy),ADX)
	./main $(strategy) $(symbol) $(x) $(n) $(adx_threshold) $(start_date) $(end_date)
endif
ifeq ($(strategy),LINEAR_REGRESSION)
	./main $(strategy) $(symbol) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
endif
ifeq ($(strategy),BEST_OF_ALL)
	./main $(strategy) $(symbol) $(start_date) $(end_date)
endif
ifeq ($(strategy),PAIRS)
ifndef stop_loss_threshold
	./main $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(start_date) $(end_date)
else
	./main $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(start_date) $(end_date) $(stop_loss_threshold)
endif
endif
temp:buildProg
	./main
buildProg: main.o parser.o ADX.o basicStart.o DMA.o DMAplus.o MACD.o RSI.o dateUtil.o
	$(Compiler) $(BinFolder)main.o $(BinFolder)parser.o $(BinFolder)ADX.o $(BinFolder)basicStart.o $(BinFolder)DMA.o $(BinFolder)DMAplus.o $(BinFolder)MACD.o $(BinFolder)RSI.o $(BinFolder)dateUtil.o -o main
main.o: $(SrcFolder)main.cpp
	$(Compiler) $(Flags) $(SrcFolder)main.cpp -o $(BinFolder)main.o
parser.o: $(SrcFolder)parser.cpp
	$(Compiler) $(Flags) $(SrcFolder)parser.cpp -o $(BinFolder)parser.o
clearAll: clearObjs clearExe
ADX.o: $(SrcFolder)ADX.cpp 
	$(Compiler) $(Flags) $(SrcFolder)ADX.cpp -o $(BinFolder)ADX.o
basicStart.o: $(SrcFolder)basicStart.cpp 
	$(Compiler) $(Flags) $(SrcFolder)basicStart.cpp -o $(BinFolder)basicStart.o
DMA.o: $(SrcFolder)DMA.cpp 
	$(Compiler) $(Flags) $(SrcFolder)DMA.cpp -o $(BinFolder)DMA.o
DMAplus.o: $(SrcFolder)DMAplus.cpp 
	$(Compiler) $(Flags) $(SrcFolder)DMAplus.cpp -o $(BinFolder)DMAplus.o
MACD.o: $(SrcFolder)MACD.cpp 
	$(Compiler) $(Flags) $(SrcFolder)MACD.cpp -o $(BinFolder)MACD.o
RSI.o: $(SrcFolder)RSI.cpp 
	$(Compiler) $(Flags) $(SrcFolder)RSI.cpp -o $(BinFolder)RSI.o
dateUtil.o: $(SrcFolder)dateUtil.cpp 
	$(Compiler) $(Flags) $(SrcFolder)dateUtil.cpp -o $(BinFolder)dateUtil.o
clearObjs:
	rm -rf $(BinFolder)*
clearExe:
	rm main