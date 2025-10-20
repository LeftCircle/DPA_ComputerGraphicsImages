# CC = g++
# CFLAGS = -Wall -g -O2 -fPIC -fopenmp -std=c++17 \
# -Iinclude \
# -I/home/leftcircle/programming/include/ 

# # Had some problems on the desktop. Not sure if L/home/leftcircle/programming/lib -wl -rpath stuff is needed
# #LDFLAGS = -L/home/leftcircle/programming/lib -Wl,-rpath,/home/leftcircle/programming/lib \

# LDFLAGS = -lOpenImageIO -lOpenImageIO_Util -lGL -lGLU -lglut 

# SRC = $(wildcard src/*.cpp)
# MAIN = main.cpp
# OBJDIR = obj
# OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(SRC)) $(OBJDIR)/main.o

# TARGET = imgviewer

# all: $(OBJDIR) $(TARGET)

# $(OBJDIR):
# 	mkdir -p $(OBJDIR)

# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# $(OBJDIR)/%.o: src/%.cpp
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(OBJDIR)/main.o: main.cpp
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean: 
# 	rm -rf $(OBJDIR) $(TARGET) $(TEST_TARGET)


CC = g++
CFLAGS = -Wall -g -O2 -fPIC -fopenmp -std=c++17 \
-Iinclude \
-I/home/leftcircle/programming/include/ 

# Had some problems on the desktop. Not sure if L/home/leftcircle/programming/lib -wl -rpath stuff is needed
LDFLAGS = -lOpenImageIO -lOpenImageIO_Util -lGL -lGLU -lglut #-L/home/leftcircle/programming/lib -Wl,-rpath,/home/leftcircle/programming/lib

SRC = $(wildcard src/*.cpp)
# Filter out tests.cpp from the main build
MAIN_SRC = $(filter-out src/tests.cpp, $(SRC))
OBJDIR = obj
MAIN_OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(MAIN_SRC)) $(OBJDIR)/main.o

# Test objects - all source files except main.cpp
TEST_SRC = $(filter-out src/main.cpp, $(SRC))
TEST_OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(TEST_SRC))

TARGET = imgviewer
TEST_TARGET = test_runner

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Main program target
$(TARGET): $(MAIN_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Test target
tests: $(OBJDIR) $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

test: tests
	./$(TEST_TARGET)

clean: 
	rm -rf $(OBJDIR) $(TARGET) $(TEST_TARGET)