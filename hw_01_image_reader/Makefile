CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -Iinclude
LDFLAGS = -lOpenImageIO

SRC = $(wildcard src/*.cpp)
MAIN = main.cpp
OBJDIR = obj
OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC)) $(OBJDIR)/main.o

TARGET = main

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)