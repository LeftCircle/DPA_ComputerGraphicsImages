CC = g++

# Update these to point to your custom include and lib directories for OpenImageIO and OpenEXR
OPEN_IMG_IO_AND_EXR_INCLUDE_PATH = /home/leftcircle/programming/include
OPEN_IMG_IO_AND_EXR_LIB_PATH = /home/leftcircle/programming/lib

CFLAGS = -Wall -g -O2 -fPIC -fopenmp -std=c++17 \
-Iinclude \
-I$(OPEN_IMG_IO_AND_EXR_INCLUDE_PATH) 

# -L/home/leftcircle/programming/lib:
#     Linker flags to find OpenImageIO and OpenEXR libs at compile time
# -Wl,-rpath,/home/leftcircle/programming/lib
#     Linker flags to find OpenImageIO and OpenEXR libs at runtime
LDFLAGS = -lOpenImageIO -lOpenImageIO_Util -lGL -lGLU -lglut \
-L$(OPEN_IMG_IO_AND_EXR_LIB_PATH) \
-Wl,-rpath,$(OPEN_IMG_IO_AND_EXR_LIB_PATH)


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