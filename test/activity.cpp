#include <iostream>
#include <vector>
#include "Instance.h"

int main(int argc, char* argv[]) {
	
	Ptr<Instance::Manager> manager = shippingInstanceManager();
	Ptr<Instance> clock = manager->instanceNew("clock", "Clock");
	
	/* Set up the network */
	vector< Ptr<Instance> > loc;
	vector< Ptr<Instance> > seg;

	// Locations
	loc.push_back( manager->instanceNew("customer1", "Customer") );
	loc.push_back( manager->instanceNew("customer2", "Customer") );

	// Segments
  seg.push_back( manager->instanceNew("ts1", "Truck segment") );
	seg.push_back( manager->instanceNew("ts2", "Truck segment") );
	seg[0]->attributeIs("length", "1");
	seg[1]->attributeIs("length", "1");

	// Connections...
	seg[0]->attributeIs("source", "customer1");
	seg[1]->attributeIs("source", "customer2");
	seg[0]->attributeIs("return segment", "ts2");

  Ptr<Instance> fleet = manager->instanceNew("Fleet", "Fleet");
  fleet->attributeIs("Truck, speed", "1");
  fleet->attributeIs("Truck, capacity", "1");
	
	// Scheduled changes
  fleet->attributeIs("Truck, speed, scheduled", "2");
  fleet->attributeIs("Truck, capacity, scheduled", "2");
  fleet->attributeIs("Start time, scheduled", "10");
  fleet->attributeIs("End time, scheduled", "20");

	// Set some segment (shipment) capacities
	seg[0]->attributeIs("capacity", "1");
	seg[1]->attributeIs("capacity", "1");
	seg[0]->attributeIs("length", "1");
	seg[1]->attributeIs("length", "1");

	Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

	conn->attributeIs("routing algorithm", "ucs");
	
	loc[0]->attributeIs("transfer rate", "4");
	loc[0]->attributeIs("shipment size", "5");
	loc[0]->attributeIs("destination", "customer2");
	
	// loc[1]->attributeIs("transfer rate", "3");
	// loc[1]->attributeIs("shipment size", "10");
	// loc[1]->attributeIs("destination", "customer1");
	
	clock->attributeIs("type", "real");

	//activityManager->nowIs(24.0);
	clock->attributeIs("now", "24");

	loc[0]->attributeIs("transfer rate", "0");

	clock->attributeIs("now", "48");

	loc[0]->attributeIs("transfer rate", "1");

	clock->attributeIs("now", "72");

	cout << "Shipments Received: " << loc[1]->attribute("shipments received") << endl;
	cout << "Average Latency: " << loc[1]->attribute("average latency") << endl;
	cout << "Total Cost: " << loc[1]->attribute("total cost") << endl;

  return 0;
}

