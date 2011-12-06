CPPFLAGS = -I.
CXXFLAGS = -Wall -g

OBJECTS = Instance.o engine/Entity.o engine/Network.o engine/Stats.o engine/Fleet.o engine/Segment.o engine/TruckSegment.o engine/PlaneSegment.o engine/BoatSegment.o engine/Location.o engine/CustomerLocation.o engine/PortLocation.o engine/TerminalLocation.o engine/TruckTerminal.o engine/BoatTerminal.o engine/PlaneTerminal.o engine/Shipment.o engine/SegmentReactor.o engine/NetworkReactor.o engine/LocationReactor.o engine/CustomerReactor.o
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
