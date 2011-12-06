CPPFLAGS = -I.
CXXFLAGS = -Wall -g

OBJECTS = ActivityImpl.o Instance.o engine/Entity.o engine/Network.o engine/Stats.o engine/Fleet.o engine/Segment.o engine/TruckSegment.o engine/PlaneSegment.o engine/BoatSegment.o engine/Location.o engine/CustomerLocation.o engine/PortLocation.o engine/TerminalLocation.o engine/TruckTerminal.o engine/BoatTerminal.o engine/PlaneTerminal.o engine/Shipment.o engine/SegmentReactor.o engine/NetworkReactor.o engine/LocationReactor.o engine/CustomerReactor.o
LIBS = fwk/BaseCollection.o fwk/BaseNotifiee.o fwk/Exception.o

default: test/test1 test/test2 test/sample test/example test/activity

test/test1:	test/test1.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test/test2: test/test2.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test/sample: test/sample.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test/example:	test/example.o $(OBJECTS) $(LIBS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	
test/activity:	test/activity.o $(OBJECTS) $(LIBS)
		$(CXX) $(CXXFLAGS) -o $@ $^
		
clean:
	rm -f test/test1 test/test1.o test/test2 test/test2.o test/sample test/sample.o test/example test/example.o test/activity test/activity.o $(OBJECTS) $(LIBS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
test1.o: test/test1.cpp Instance.h PtrInterface.h Ptr.h
