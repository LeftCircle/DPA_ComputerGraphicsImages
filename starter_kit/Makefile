

OFILES = \
         base/Matrix.o \
         base/LinearAlgebra.o \
	 base/StarterViewer.o

ROOTDIR = .
LIB = $(ROOTDIR)/lib/libstarter.a 

GLLDFLAGS     = -lglut -lGL -lm -lGLU

CXX = g++ -Wall -g -O2 -fPIC $(DEFINES) -fopenmp -std=c++14

INCLUDES =  -I ./include/ -I /usr/local/include/ -I/usr/include/ 


.C.o:
	$(CXX) -c $(INCLUDES) $< -o $@

base: $(OFILES)
	ar rv $(LIB) $?
	$(CXX) base/simple_viewer.C $(INCLUDES)  -L./lib -lstarter $(GLLDFLAGS)  -o bin/simple_viewer

clean:
	rm -rf bin/simple_viewer *.o base/*.o base/*~ include/*~ $(LIB)  *~ 



