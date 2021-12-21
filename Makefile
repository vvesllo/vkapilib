path = src/bot.cxx src/vkapilib.cxx
exit_name = main.exe

all:
	g++ $(path) -Wall -std=c++17 -o $(exit_name) -lcurl

clear:
	-rm *.exe
	-rm *.out
