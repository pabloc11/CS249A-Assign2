CPPFLAGS = -I.
CXXFLAGS = -Wall -g

OBJECTS = Instance.o Entity.o Network.o Stats.o Fleet.o Segment.o TruckSegment.o PlaneSegment.o BoatSegment.o Location.o CustomerLocation.o PortLocation.o
LIBS = fwk/BaseCollection.o fwk/BaseNotifiee.o fwk/Exception.o

default:	test1 example

test1:	test1.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o example example.o $(OBJECTS) $(LIBS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
