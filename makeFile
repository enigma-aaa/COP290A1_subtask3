Compiler=g++
Flags=-c -Wall
BinFolder=./bin/
SrcFolder=./src/

all:buildProg

buildProg: main.o
	$(Compiler) main.o -o main
main.o: main.cpp
	$(Compiler) $(Flags) $(SrcFolder)main.cpp -o $(BinFolder)main.o
clearAll: clearObjs clearExe

clearObjs:
	rm -rf $(BinFolder)*
clearExe:
	rm main