

APP = undistort
SRCS = src/undistort.cpp

CFLAGS = -ggdb


default: $(APP)


test: $(APP)
	./$(APP) --verbose --xml data/better_coverage.xml data/G0022591.JPG

$(APP): $(SRCS:.cpp=.o)
	$(CXX) $(LDFLAGS) $(CFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

PHONY: clean

clean:
	rm -f $(SRCS:.cpp=.o)

