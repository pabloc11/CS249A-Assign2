CPPFLAGS = -I.
CXXFLAGS = -Wall -g

OBJECTS = Instance.o Entity.o Network.o Stats.o Fleet.o Segment.o TruckSegment.o PlaneSegment.o BoatSegment.o Location.o CustomerLocation.o PortLocation.o TerminalLocation.o TruckTerminal.o BoatTerminal.o PlaneTerminal.o Shipment.o
LIBS = fwk/BaseCollection.o fwk/BaseNotifiee.o fwk/Exception.o

default: test1 example test2 sample

test1:	test1.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test2: test2.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

sample: sample.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

example:	example.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o test2 test2.o sample sample.o example example.o $(OBJECTS) $(LIBS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
