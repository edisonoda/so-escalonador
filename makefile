CXX=g++
LDLIBS=-lncurses

TARGET=escalonador

all: $(TARGET)

$(TARGET): main.cpp core/*.cpp scheduler/*.cpp ui/*.cpp utils/*.*
	$(CXX) main.cpp core/*.cpp scheduler/*.cpp ui/*.cpp utils/*.* -o $(TARGET) $(LDLIBS)

run: $(TARGET)
	./$(TARGET)