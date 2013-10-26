# ccconf _test.so -python2.7 CXX=g++48 CXXFLAGS+=-fPIC -std=c++11 -Wall -g -I/usr/local/include/python2.7 LDFLAGS+=-shared -L/usr/local/lib
LDFLAGS  = -lpython2.7  -shared -L/usr/local/lib  
CXXFLAGS = -fPIC -std=c++11 -Wall -g -I/usr/local/include/python2.7  
CXX      = g++48  

.PHONY : all clean
all : _test.so
clean :
	rm -f _test.so _test.o tags

tags : *.h _test.cc 
	ctags *.h _test.cc 

_test.so : _test.o
	${CXX} ${LDFLAGS} -o _test.so _test.o
_test.o: _test.cc ../pymemory.h
