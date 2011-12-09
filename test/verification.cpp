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
	loc.push_back( manager->instanceNew("terminal1", "Truck terminal") );
	loc.push_back( manager->instanceNew("terminal2", "Truck terminal") );

	// Segments
	seg.push_back( manager->instanceNew("ts1", "Truck segment") );
	seg.push_back( manager->instanceNew("ts2", "Truck segment") );
	seg.push_back( manager->instanceNew("ts3", "Truck segment") );
	seg.push_back( manager->instanceNew("ts4", "Truck segment") );
	seg.push_back( manager->instanceNew("ts5", "Truck segment") );
	seg.push_back( manager->instanceNew("ts6", "Truck segment") );
	seg.push_back( manager->instanceNew("ts7", "Truck segment") );
	seg.push_back( manager->instanceNew("ts8", "Truck segment") );
	seg[0]->attributeIs("length", "2");
	seg[1]->attributeIs("length", "2");
	seg[2]->attributeIs("length", "2");
	seg[3]->attributeIs("length", "2");
	seg[4]->attributeIs("length", "1");
	seg[5]->attributeIs("length", "1");
	seg[6]->attributeIs("length", "1");
	seg[7]->attributeIs("length", "1");

	// Connections...
	seg[0]->attributeIs("source", "customer1");
	seg[1]->attributeIs("source", "terminal1");
	seg[0]->attributeIs("return segment", "ts2");

	seg[2]->attributeIs("source", "terminal1");
	seg[3]->attributeIs("source", "customer2");
	seg[2]->attributeIs("return segment", "ts4");

	seg[4]->attributeIs("source", "customer1");
	seg[5]->attributeIs("source", "terminal2");
	seg[4]->attributeIs("return segment", "ts6");

	seg[6]->attributeIs("source", "terminal2");
	seg[7]->attributeIs("source", "customer2");
	seg[6]->attributeIs("return segment", "ts8");

	// Set some segment (shipment) capacities
	seg[0]->attributeIs("capacity", "1");
	seg[1]->attributeIs("capacity", "1");
	seg[2]->attributeIs("capacity", "1");
	seg[3]->attributeIs("capacity", "1");
	seg[4]->attributeIs("capacity", "1");
	seg[5]->attributeIs("capacity", "1");
	seg[6]->attributeIs("capacity", "1");
	seg[7]->attributeIs("capacity", "1");

	Ptr<Instance> fleet = manager->instanceNew("Fleet", "Fleet");
	fleet->attributeIs("Truck, speed", "1");
	fleet->attributeIs("Truck, capacity", "1");

	Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

	conn->attributeIs("routing algorithm", "dfs");

	loc[0]->attributeIs("transfer rate", "2");
	loc[0]->attributeIs("shipment size", "5");
	loc[0]->attributeIs("destination", "customer2");

	loc[1]->attributeIs("transfer rate", "6");
	loc[1]->attributeIs("shipment size", "10");
	loc[1]->attributeIs("destination", "customer1");

	clock->attributeIs("now", "24");

	cout << "Shipments Received at Customer 2: " << loc[1]->attribute("shipments received") << endl;
	cout << "Average Latency at Customer 2: " << loc[1]->attribute("average latency") << endl;
	cout << "Total Cost at Customer 2: " << loc[1]->attribute("total cost") << endl;
	cout << "Shipments Received at Customer 1: " << loc[0]->attribute("shipments received") << endl;
	cout << "Average Latency at Customer 1: " << loc[0]->attribute("average latency") << endl;
	cout << "Total Cost at Customer 1: " << loc[0]->attribute("total cost") << endl;

  return 0;
}

