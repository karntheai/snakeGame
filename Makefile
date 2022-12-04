CXX		  := g++
CXX_FLAGS := -Wall -Wextra -Wno-unused-parameter -std=c++17 -ggdb -Wno-missing-braces -Wunused-result

BIN := bin
OBJ := objects
EXECUTABLE	:= main
MKDIR_P = mkdir

all: compile  link	run  

LIB :=lib
INCLUDE :=include

LIBRARIES	:= -lraylib -lm -lpthread -DPLATFORM_DESKTOP

directories: ${BIN}

${BIN}:
	${MKDIR_P} ${BIN}

run: clean all
	clear
	./$(EXECUTABLE)

compile:

# TODO: Find a method to add all the objects in the bin folder

	g++ -Iinclude -I$(INCLUDE) -c main.cpp $(LIB)/*.cpp


link:
	$(CXX) *.o -I$(INCLUDE) -o main   $(LIBRARIES)

clean:
	-rm *.o


#  Make sure to comment this before pushing it to github
# temp:
# 	g++ -Isrc/include -c temp.cpp

# 	$(CXX) temp.o  -Isrc/include -o temp -Lsrc/lib $(LIBRARIES)
