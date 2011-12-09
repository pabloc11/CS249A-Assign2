#include <iostream>
#include <vector>
#include <sstream>
#include "Instance.h"

string itoa(int i){
	std::string s;
	std::stringstream out;
	out << i;
	s = out.str();
	return s;
}

int main(int argc, char* argv[]) {

	Ptr<Instance::Manager> manager = shippingInstanceManager();
	Ptr<Instance> clock = manager->instanceNew("clock", "Clock");

	/* Set up the network */
	vector< Ptr<Instance> > customers;
	vector< Ptr<Instance> > terminals;
	vector< Ptr<Instance> > segments;

	// Locations
	for(int i = 0; i < 101; i++)
		customers.push_back( manager->instanceNew("customer" + itoa(i), "Customer") );
	for(int i = 0; i < 11; i++)
		terminals.push_back( manager->instanceNew("terminal" + itoa(i), "Truck terminal") );

	// Segments
	for(int i = 0; i < 222; i++) {
		segments.push_back( manager->instanceNew("segment" + itoa(i), "Truck segment"));
		segments[i]->attributeIs("length", "1");
	}

	// Connections...
	for(int i = 0; i < 100; i++) {
		segments[i]->attributeIs("source", "customer" + itoa(i));
		segments[i+100]->attributeIs("source", "terminal" + itoa(i / 10));
		segments[i]->attributeIs("return segment", "segment" + itoa(i+100));
	}

	for(int i = 0; i < 10; i++) {
		segments[i+200]->attributeIs("source", "terminal" + itoa(i));
		segments[i+210]->attributeIs("source", "terminal10");
		segments[i+200]->attributeIs("return segment", "segment" + itoa(i+210));
	}

	segments[220]->attributeIs("source", "terminal10");
	segments[221]->attributeIs("source", "customer100");
	segments[220]->attributeIs("return segment", "segment221");

	// Set some segment (shipment) capacities
	for(int i = 0; i < 222; i++)
		segments[i]->attributeIs("capacity", "1");

	Ptr<Instance> fleet = manager->instanceNew("Fleet", "Fleet");
	fleet->attributeIs("Truck, speed", "1");
	fleet->attributeIs("Truck, capacity", "1");

	Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

	conn->attributeIs("routing algorithm", "ucs");

	for(int i = 0; i < 100; i++) {
		customers[i]->attributeIs("transfer rate", "1");
		customers[i]->attributeIs("shipment size", "1");
		customers[i]->attributeIs("destination", "customer100");
	}

	clock->attributeIs("now", "24");

	cout << "Shipments Received at Customer 100: " << customers[100]->attribute("shipments received") << endl;
	cout << "Average Latency at Customer 100: " << customers[100]->attribute("average latency") << endl;
	cout << "Total Cost at Customer 100: " << customers[100]->attribute("total cost") << endl;
  return 0;
}

