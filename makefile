Compiler=g++
Flags=-std=c++20 -c -g
BinFolder=./bin/
SrcFolder=./src/

all:main
ifeq ($(strategy),BASIC)
	./main $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date)
endif
ifeq ($(strategy),DMA)
	./main $(strategy) $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)
endif
ifeq ($(strategy),DMA++)
	echo "DMA++"
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
main: ./bin/main.o ./bin/parser.o ./bin/ADX.o ./bin/basicStart.o ./bin/DMA.o ./bin/DMAplus.o ./bin/MACD.o ./bin/RSI.o ./bin/dateUtil.o ./bin/linearRegNorm.o ./bin/Best.o ./bin/Pairs.o
	$(Compiler) $(BinFolder)main.o $(BinFolder)parser.o $(BinFolder)ADX.o $(BinFolder)basicStart.o $(BinFolder)DMA.o $(BinFolder)DMAplus.o $(BinFolder)MACD.o $(BinFolder)RSI.o $(BinFolder)dateUtil.o $(BinFolder)linearRegNorm.o $(BinFolder)Best.o $(BinFolder)Pairs.o -o main
./bin/main.o: $(SrcFolder)main.cpp
	$(Compiler) $(Flags) $(SrcFolder)main.cpp -o $(BinFolder)main.o
./bin/parser.o: $(SrcFolder)parser.cpp
	$(Compiler) $(Flags) $(SrcFolder)parser.cpp -o $(BinFolder)parser.o
clearAll: clearObjs clearExe
./bin/ADX.o: $(SrcFolder)ADX.cpp 
	$(Compiler) $(Flags) $(SrcFolder)ADX.cpp -o $(BinFolder)ADX.o
./bin/basicStart.o: $(SrcFolder)basicStart.cpp 
	$(Compiler) $(Flags) $(SrcFolder)basicStart.cpp -o $(BinFolder)basicStart.o
./bin/DMA.o: $(SrcFolder)DMA.cpp 

	$(Compiler) $(Flags) $(SrcFolder)DMA.cpp -o $(BinFolder)DMA.o
./bin/DMAplus.o: $(SrcFolder)DMAplus.cpp 

	$(Compiler) $(Flags) $(SrcFolder)DMAplus.cpp -o $(BinFolder)DMAplus.o
./bin/MACD.o: $(SrcFolder)MACD.cpp 
	$(Compiler) $(Flags) $(SrcFolder)MACD.cpp -o $(BinFolder)MACD.o
./bin/RSI.o: $(SrcFolder)RSI.cpp 
	$(Compiler) $(Flags) $(SrcFolder)RSI.cpp -o $(BinFolder)RSI.o
./bin/dateUtil.o: $(SrcFolder)dateUtil.cpp 
	$(Compiler) $(Flags) $(SrcFolder)dateUtil.cpp -o $(BinFolder)dateUtil.o
./bin/linearRegNorm.o: $(SrcFolder)linearRegNorm.cpp 
	$(Compiler) $(Flags) $(SrcFolder)linearRegNorm.cpp -o $(BinFolder)linearRegNorm.o
./bin/LinearRegression.o: $(SrcFolder)LinearRegression.cpp 
	$(Compiler) $(Flags) $(SrcFolder)LinearRegression.cpp -o $(BinFolder)LinearRegression.o
./bin/Best.o: $(SrcFolder)Best.cpp 
	$(Compiler) $(Flags) $(SrcFolder)Best.cpp -o $(BinFolder)Best.o
./bin/Pairs.o: $(SrcFolder)Pairs.cpp 
	$(Compiler) $(Flags) $(SrcFolder)Pairs.cpp -o $(BinFolder)Pairs.o
clearObjs:
	rm -rf $(BinFolder)*
clearExe:
	rm main