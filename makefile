CXX=g++
LDLIBS=-lncurses

TARGET=escalonador

all: $(TARGET)

$(TARGET): main.cpp core/*.cpp core/clock/*.cpp scheduler/*.cpp ui/*.cpp utils/constants.hpp
	$(CXX) main.cpp core/*.cpp core/clock/*.cpp scheduler/*.cpp ui/*.cpp utils/constants.hpp -o $(TARGET) $(LDLIBS)

run: $(TARGET)
	./$(TARGET)