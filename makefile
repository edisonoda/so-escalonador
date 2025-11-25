CXX=g++
LDLIBS=-lncurses

TARGET=escalonador

all: $(TARGET)

$(TARGET): main.cpp core/*.cpp ui/*.cpp
	$(CXX) main.cpp core/*.cpp ui/*.cpp -o $(TARGET) $(LDLIBS)

run: $(TARGET)
	./$(TARGET)