

APP = undistort
SRCS = src/undistort.cpp

CFLAGS = -ggdb
LDFLAGS = 

LIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_calib3d


default: $(APP)


test: $(APP)
	./$(APP) --verbose --xml sample/better_coverage.xml sample/G0022591.JPG

$(APP): $(SRCS:.cpp=.o)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $< $(LIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

PHONY: clean

clean:
	rm -f $(SRCS:.cpp=.o)

