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
buildProg: main.o parser.o
	$(Compiler) $(BinFolder)main.o $(BinFolder)parser.o -o main
main.o: $(SrcFolder)main.cpp
	$(Compiler) $(Flags) $(SrcFolder)main.cpp -o $(BinFolder)main.o
parser.o: $(SrcFolder)parser.cpp
	$(Compiler) $(Flags) $(SrcFolder)parser.cpp -o $(BinFolder)parser.o
clearAll: clearObjs clearExe

clearObjs:
	rm -rf $(BinFolder)*
clearExe:
	rm main