CXX = c++
GDIR= ./
CFLAGS = -std=c++11 -Wall -Wextra
IFLAGS = -I inc -I misc
DFLAGS = 

all: testmain.x	timingmain.x
	

debug: DFLAGS = -D DEBUG
debug: all


src/bst.o:  src/bst.cc 
	$(CXX) -c -o $@ $^ $(CFLAGS) $(IFLAGS) $(DFLAGS)


timingmain.x: timingmain.o	src/bst.o
	$(CXX) -o $@ $^ $(CFLAGS) $(IFLAGS) $(DFLAGS)


timingmain.o: timingmain.cc
	$(CXX) -c -o $@ $^ $(CFLAGS) $(IFLAGS) $(DFLAGS)


testmain.x: testmain.o	src/bst.o
	$(CXX) -o $@ $^ $(CFLAGS) $(IFLAGS) $(DFLAGS)


testmain.o: testmain.cc
	$(CXX) -c -o $@ $^ $(CFLAGS) $(IFLAGS) $(DFLAGS)


doc: Doxygen/doxy.in
	doxygen $^


clean:
	@rm -rf *~ *.o src/*.o libbst.so html latex testmain.x timingmain.x

.PHONY: clean

