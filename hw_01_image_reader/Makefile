CC = g++
CFLAGS = -Wall -g -O2 -fPIC -fopenmp -Wextra -std=c++17 \
-Iinclude \
-I/home/leftcircle/programming/include/ 

LDFLAGS = -lOpenImageIO -lOpenImageIO_Util -lGL -lGLU -lglut


SRC = $(wildcard src/*.cpp) $(wildcard src/starter_kit/*.C)
MAIN = main.cpp
OBJDIR = obj
OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC)) $(OBJDIR)/main.o

TARGET = imgviewer

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