CXX=g++
CXXFLAGS=-Wall -Wextra -Wpedantic -g -I./common -I./src/include -I./backends/vim-backend/include -std=c++17 -g
LINKFLAGS= -L./backends/vim-backend/ -lVimBackend -lncurses -lpanel
TARGET=midnight-commander.app

BASESOURCES=$(wildcard ./src/*.cpp)
SOURCES= $(BASESOURCES)
TARGETS= $(SOURCES:.cpp=.o)
BUILDSOUP = $(wildcard ./build/*.o)

.PHONY: sources targets all target clean rebuild
.SUFFIXES:

all: target
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(BUILDSOUP) $(LINKFLAGS) 

target: $(TARGETS)

rebuild: clean all

clean:
	rm -rf $(BUILDSOUP)
	rm -f $(TARGET)

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o build/$(notdir $@)

sources:
	@echo $(SOURCES)

targets:
	@echo $(TARGETS)
