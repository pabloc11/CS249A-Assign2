#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include "Instance.h"
#include <assert.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {
    Ptr<Instance::Manager> manager = shippingInstanceManager();

    if (manager == NULL) {
        cerr << "Unexpected NULL manager." << endl;
        return 1;
    }

    Ptr<Instance> stats = manager->instanceNew("myStats", "Stats");

    if (stats == NULL) {
        cerr << "Unexpected NULL stats." << endl;
        return 1;
    }

    Ptr<Instance> fleet = manager->instanceNew("myFleet", "Fleet");

    if (fleet == NULL) {
        cerr << "Unexpected NULL." << endl;
        return 1;
    }

    fleet->attributeIs("Boat, speed", "60");
    assert(fleet->attribute("Boat, speed") == "60.00");
    fleet->attributeIs("Truck, capacity", "50");
    assert(fleet->attribute("Truck, capacity") == "50");
    fleet->attributeIs("Plane, cost", "20");
    assert(fleet->attribute("Plane, cost") == "20.00");

    // --- Create instances
    // -- Locations
    // customers
    Ptr<Instance> customer1 = manager->instanceNew("customer1", "Customer");  
    Ptr<Instance> customer2 = manager->instanceNew("customer2", "Customer");  
    // ports
    Ptr<Instance> port1 = manager->instanceNew("port1", "Port");  

    if (customer1 == NULL || customer2 == NULL || port1 == NULL) {
        cerr << "Unexpected NULL customer or port." << endl;
  return 1;
    }

    // -- Segments
    // boat
    Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");  
    Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment");  
    // truck
    Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");  
    Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");  

    if (boatSeg1 == NULL || boatSeg2 == NULL || truckSeg1 == NULL || truckSeg2 == NULL) {
        cerr << "Unexpected NULL segment." << endl;
        return 1;
    }

    // connections
    // customer1 <---> port1
    truckSeg1->attributeIs("source", "customer1");
    assert(truckSeg1->attribute("source") == "customer1");
    assert(customer1->attribute("segment1") == "truckSeg1");
    truckSeg2->attributeIs("source", "port1");
    assert(truckSeg2->attribute("source") == "port1");
    assert(port1->attribute("segment1") == "truckSeg2");
    truckSeg1->attributeIs("return segment", "truckSeg2");
    assert(truckSeg1->attribute("return segment") == "truckSeg2");
    assert(truckSeg2->attribute("return segment") == "truckSeg1");
    
    // customer2 <---> port1
    boatSeg1->attributeIs("source", "customer2");
    assert(boatSeg1->attribute("source") == "customer2");
    assert(customer2->attribute("segment1") == "boatSeg1");
    boatSeg2->attributeIs("source", "port1");
    assert(boatSeg2->attribute("source") == "port1");
    assert(port1->attribute("segment2") == "boatSeg2");
    boatSeg1->attributeIs("return segment", "boatSeg2");
    assert(boatSeg1->attribute("return segment") == "boatSeg2");
    assert(boatSeg2->attribute("return segment") == "boatSeg1");

    // -- Segment lengths
    boatSeg1->attributeIs("length", "400");
    assert(boatSeg1->attribute("length") == "400.00");
    boatSeg2->attributeIs("length", "400");
    assert(boatSeg2->attribute("length") == "400.00");
    truckSeg1->attributeIs("length", "900");
    assert(truckSeg1->attribute("length") == "900.00");
    truckSeg2->attributeIs("length", "900");
    assert(truckSeg2->attribute("length") == "900.00");

    // -- Segment difficulties
    boatSeg1->attributeIs("difficulty", "1");
    assert(boatSeg1->attribute("difficulty") == "1.00");
    boatSeg2->attributeIs("difficulty", "1");
    assert(boatSeg2->attribute("difficulty") == "1.00");
    truckSeg1->attributeIs("difficulty", "1");
    assert(truckSeg1->attribute("difficulty") == "1.00");
    truckSeg2->attributeIs("difficulty", "1");
    assert(truckSeg2->attribute("difficulty") == "1.00");
    
    // -- Segment expedite support
    boatSeg1->attributeIs("expedite support", "yes");
    assert(boatSeg1->attribute("expedite support") == "yes");
    boatSeg2->attributeIs("expedite support", "yes");
    assert(boatSeg2->attribute("expedite support") == "yes");
    truckSeg1->attributeIs("expedite support", "yes");
    assert(truckSeg1->attribute("expedite support") == "yes");
    truckSeg2->attributeIs("expedite support", "yes");
    assert(truckSeg2->attribute("expedite support") == "yes");
    truckSeg2->attributeIs("expedite support", "no");
    assert(truckSeg2->attribute("expedite support") == "no");
    truckSeg2->attributeIs("expedite support", "yes");
    assert(truckSeg2->attribute("expedite support") == "yes");

    // -- Connectivity queries

    Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

    if (conn == NULL) {
        cerr << "Unexpected NULL conn." << endl;
        return 1;
    }

    cout << "**** explore customer1 : distance 1500 ****" << endl;
    cout << conn->attribute("explore customer1 : distance 1500") << endl;
    cout << endl;

    cout << "*** connect customer2 : customer1 ****" << endl;
    cout << conn->attribute("connect customer2 : customer1") << endl;
    cout << endl;

    manager->instanceDel("boatSeg1");
    cout << boatSeg2->attribute("return segment") << endl;
    manager->instanceDel("port1");
    assert(truckSeg2->attribute("source") == "");
    assert(boatSeg2->attribute("source") == "");

    // -- Statistics queries
    cout << "===== Stats attributes =====" << endl;
    cout << " --- Segments --- " << endl;
    cout << "# Truck segments : " << stats->attribute("Truck segment") << endl;
    assert(stats->attribute("Truck segment") == "2");
    cout << "# Boat segments  : " << stats->attribute("Boat segment") << endl;
    assert(stats->attribute("Boat segment") == "1");
    cout << "# Plane segments : " << stats->attribute("Plane segment") << endl;
    assert(stats->attribute("Plane segment") == "0");
    cout << "Expediting %     : " << stats->attribute("expedite percentage") << endl;
    assert(stats->attribute("expedite percentage") == "100.00");
    cout << " --- Terminals --- " << endl;
    cout << "# Truck terminals: " << stats->attribute("Truck terminal") << endl;
    assert(stats->attribute("Truck terminal") == "0");
    cout << "# Boat terminals : " << stats->attribute("Boat terminal") << endl;
    assert(stats->attribute("Boat terminal") == "0");
    cout << "# Plane terminals: " << stats->attribute("Plane terminal") << endl;
    assert(stats->attribute("Plane terminal") == "0");
    cout << " --- Ports and customers --- " << endl;
    cout << "# Ports          : " << stats->attribute("Port") << endl;
    assert(stats->attribute("Port") == "0");
    cout << "# Customers      : " << stats->attribute("Customer") << endl;
    assert(stats->attribute("Customer") == "2");

    cerr << "Done!" << endl;

    return 0;
}
