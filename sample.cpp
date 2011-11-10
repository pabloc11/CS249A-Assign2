#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include "Instance.h"

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

    fleet->attributeIs("Boat, speed", "30");
    fleet->attributeIs("Truck, speed", "70");
    fleet->attributeIs("Plane, speed", "300");

    fleet->attributeIs("Boat, cost", "20");
    fleet->attributeIs("Truck, cost", "10");
    fleet->attributeIs("Plane, cost", "100");
    // --- Create instances
    // -- Locations
    // customers
    Ptr<Instance> customer1 = manager->instanceNew("customer1", "Customer");
    Ptr<Instance> customer2 = manager->instanceNew("customer2", "Customer");

    // Truck Terminal
    Ptr<Instance> truckTerminal1 = manager->instanceNew("truckterminal1", "Truck terminal");

    // Boat Terminal
    Ptr<Instance> boatTerminal1 = manager->instanceNew("boatterminal1", "Boat terminal");

    // Plane Terminal
    Ptr<Instance> planeTerminal1 = manager->instanceNew("planeterminal1", "Plane terminal");

    if (customer1 == NULL || customer2 == NULL || truckTerminal1 == NULL || boatTerminal1 == NULL
        || planeTerminal1 == NULL) {
        cerr << "Unexpected NULL customer or port." << endl;
    return 1;
    }

    // -- Segments
    // boat
    Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");
    Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment");
    Ptr<Instance> boatSeg3 = manager->instanceNew("boatSeg3", "Boat segment");
    Ptr<Instance> boatSeg4 = manager->instanceNew("boatSeg4", "Boat segment");
    // truck
    Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");
    Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");
    Ptr<Instance> truckSeg3 = manager->instanceNew("truckSeg3", "Truck segment");
    Ptr<Instance> truckSeg4 = manager->instanceNew("truckSeg4", "Truck segment");


    // plane
    Ptr<Instance> planeSeg1 = manager->instanceNew("planeSeg1", "Plane segment");
    Ptr<Instance> planeSeg2 = manager->instanceNew("planeSeg2", "Plane segment");
    Ptr<Instance> planeSeg3 = manager->instanceNew("planeSeg3", "Plane segment");
    Ptr<Instance> planeSeg4 = manager->instanceNew("planeSeg4", "Plane segment");

    // connections
    // customer1 <---> truck terminal 1
    truckSeg1->attributeIs("source", "customer1");
    truckSeg2->attributeIs("source", "truckterminal1");
    truckSeg1->attributeIs("return segment", "truckSeg2");

    // customer2 <---> truck terminal 1
    truckSeg3->attributeIs("source", "customer2");
    truckSeg4->attributeIs("source", "truckterminal1");
    truckSeg3->attributeIs("return segment", "truckSeg4");

    // boat terminal 1 <---> customer1
    boatSeg1->attributeIs("source", "boatterminal1");
    boatSeg2->attributeIs("source", "customer1");
    boatSeg1->attributeIs("return segment", "boatSeg2");

    // boat terminal 1 <---> customer 2
    boatSeg3->attributeIs("source", "boatterminal1");
    boatSeg4->attributeIs("source", "customer2");
    boatSeg3->attributeIs("return segment", "boatSeg4");

    //  plane terminal 1 <---> customer1
    planeSeg1->attributeIs("source", "planeterminal1");
    planeSeg2->attributeIs("source", "customer1");
    planeSeg1->attributeIs("return segment", "planeSeg2");    
    //  plane terminal 1 <---> customer2
    planeSeg3->attributeIs("source", "planeterminal1");
    planeSeg4->attributeIs("source", "customer2");
    planeSeg3->attributeIs("return segment", "planeSeg4");

    // -- Segment lengths
    boatSeg1->attributeIs("length", "400");
    boatSeg2->attributeIs("length", "300");
    boatSeg3->attributeIs("length", "200");
    boatSeg4->attributeIs("length", "500");
    truckSeg1->attributeIs("length", "800");
    truckSeg2->attributeIs("length", "900");
    truckSeg3->attributeIs("length", "200");
    truckSeg4->attributeIs("length", "400");
    planeSeg1->attributeIs("length", "500");
    planeSeg2->attributeIs("length", "600");
    planeSeg3->attributeIs("length", "800");
    planeSeg4->attributeIs("length", "1000");

    // -- Segment difficulties
    boatSeg1->attributeIs("difficulty", "1");
    boatSeg2->attributeIs("difficulty", "1.4");
    boatSeg3->attributeIs("difficulty", "1.8");
    boatSeg4->attributeIs("difficulty", "2");
    truckSeg1->attributeIs("difficulty", "2");
    truckSeg2->attributeIs("difficulty", "3");
    truckSeg3->attributeIs("difficulty", "4.5");
    truckSeg4->attributeIs("difficulty", "5");
    planeSeg1->attributeIs("difficulty", "1");
    planeSeg2->attributeIs("difficulty", "1.5");
    planeSeg3->attributeIs("difficulty", "1.5");
    planeSeg4->attributeIs("difficulty", "1.7");

    // -- Segment expedite support
    boatSeg1->attributeIs("expedite support", "yes");
    boatSeg2->attributeIs("expedite support", "yes");
    boatSeg3->attributeIs("expedite support", "yes");
    boatSeg4->attributeIs("expedite support", "yes");
    truckSeg1->attributeIs("expedite support", "yes");
    truckSeg2->attributeIs("expedite support", "yes");
    truckSeg3->attributeIs("expedite support", "yes");
    truckSeg4->attributeIs("expedite support", "yes");
    planeSeg1->attributeIs("expedite support", "yes");
    planeSeg2->attributeIs("expedite support", "yes");
    planeSeg3->attributeIs("expedite support", "yes");
    planeSeg4->attributeIs("expedite support", "yes");

    

    // -- Connectivity queries
    Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

    if (conn == NULL) {
        cerr << "Unexpected NULL conn." << endl;
        return 1;
    }

    cout << "**** explore customer1 : expedited****" << endl;
    cout << conn->attribute("explore customer1 : expedited distance 10000 time 1000") << endl;
    cout << endl;

    cout << "*** connect costomer2 : truckterminal1 ****" << endl;
    cout << conn->attribute("connect customer2 : truckterminal1") << endl;
    cout << endl;

      
    
    
    

    // -- Statistics queries
    cout << "===== Stats attributes =====" << endl;
    cout << " --- Segments --- " << endl;
    cout << "# Truck segments : " << stats->attribute("Truck segment") << endl;
    cout << "# Boat segments : " << stats->attribute("Boat segment") << endl;
    cout << "Expediting %     : " << stats->attribute("expedite percentage") << endl;
    cout << " --- Terminals --- " << endl;
    cout << "# Plane terminals: " << stats->attribute("Plane terminal") << endl;
    cout << "# Truck terminals: " << stats->attribute("Truck terminal") << endl;
    cout << "# Boat terminals: " << stats->attribute("Boat terminal") << endl;
    cout << " --- Ports and customers --- " << endl;
    cout << "# Ports          : " << stats->attribute("Port") << endl;
    cout << "# Customer          : " << stats->attribute("Customer") << endl;

    cerr << "Done!" << endl;

    return 0;
}
