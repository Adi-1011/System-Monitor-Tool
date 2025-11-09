CXX = g++
CXXFLAGS = -std=c++17 -Wall
SRC = src/main.cpp src/SystemMonitor.cpp
OUT = build/system_monitor

all:
	$(CXX) $(SRC) $(CXXFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
