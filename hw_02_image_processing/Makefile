CC = g++
CFLAGS = -Wall -g -O2 -fPIC -fopenmp -std=c++17 \
-Iinclude \
-I/home/leftcircle/programming/include/ 

# Had some problems on the desktop. Not sure if L/home/leftcircle/programming/lib -wl -rpath stuff is needed
#LDFLAGS = -L/home/leftcircle/programming/lib -Wl,-rpath,/home/leftcircle/programming/lib \

LDFLAGS = -lOpenImageIO -lOpenImageIO_Util -lGL -lGLU -lglut 



SRC = $(wildcard src/*.cpp)
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