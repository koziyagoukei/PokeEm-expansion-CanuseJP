CXX ?= g++

CXXFLAGS := -Wall -std=c++17 -O2

SRCS := json11.cpp mapjson.cpp

HEADERS := mapjson.h ../../include/constants/map_buffers.h ../../include/constants/battle_pyramid.h

ifeq ($(OS),Windows_NT)
EXE := .exe
else
EXE :=
endif

.PHONY: all clean

all: mapjson$(EXE)
	@:

mapjson$(EXE): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $@ $(LDFLAGS)

clean:
	$(RM) mapjson mapjson.exe
