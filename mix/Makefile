include ./Makefile.ini # PYTHON e BOOST vars
CXX = c++
LFLAGS = -L$(BOOST_PYTHON) $(BOOST_PYTHON_LIB)
CXXFLAGS = -std=c++11 -Wall -Wextra 
IFLAGS = -I../cpp/inc -I../cpp/misc


all: _bst.so


bst.o:	../cpp/src/bst.cc
	$(CXX)  $(CXXFLAGS) -I$(PYTHON) $(IFLAGS) -fPIC -c $^ -o $@
	
_bst.o: _bst.cc 
	$(CXX)  $(CXXFLAGS) -I$(PYTHON) $(IFLAGS) -fPIC -c $^ -o $@

_bst.so: bst.o	_bst.o
	$(CXX) -shared -Wl,-soname,"$@" -o $@ $^ $(LFLAGS)


clean:
	rm -f *.so *.o *~

.PHONY: clean

