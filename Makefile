# OpenCV Path (Change this for your own)
CV_PATH = /usr/local/opt/opencv3

# Main Configurations
CXX=g++
CXXFLAGS=-std=c++14 -Wall -pedantic
LDFLAGS=-L$(CV_PATH)/lib
INCLUDES=-I$(CV_PATH)/include
LIBS=-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
SOURCES=demo.cpp deNoise.cpp
#Objs are all the sources, with .cpp replaced by .o
OBJS=$(SOURCES:.cpp=.o)

all: demo

 # Compile the binary 'demo' by calling the compiler with cflags, lflags, and any libs (if defined) and the list of objects.
demo: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

# Get a .o from a .cpp by calling compiler with cflags and includes (if defined)
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<

# clean up
clean:
	rm *.o demo

test:
	./demo org.jpg rendered.jpg

