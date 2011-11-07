CPPFLAGS = -I.
CXXFLAGS = -Wall -g

OBJECTS = Instance.o Entity.o Network.o Stats.o Fleet.o
LIBS = fwk/BaseCollection.o fwk/BaseNotifiee.o fwk/Exception.o

default:	test1 example

test1:	test1.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o $(OBJECTS) $(LIBS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
Network.o: Network.cpp Engine.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h