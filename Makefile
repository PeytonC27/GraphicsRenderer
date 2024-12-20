CXX = g++
CXXFLAGS = -std=c++17 -Wall
INCLUDE = -I./SDL2/include
LIBS = -L./SDL2/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
SRCS = ./src/main.cpp ./src/framework.cpp
OBJS = main.o framework.o vector2.o rgba.o
TARGET = main.exe

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(INCLUDE) $(LIBS) -o $(TARGET)

main.o: ./src/main.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

framework.o: ./src/framework.cpp ./include/framework.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

vector2.o: ./src/vector2.cpp ./include/vector2.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

rgba.o: ./src/rgba.cpp ./include/rgba.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

build_and_run:
	mingw32-make
	./$(TARGET)

clean:
	del /q $(OBJS) $(TARGET)


