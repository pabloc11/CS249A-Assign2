#include <string>
#include <ostream>
#include <iostream>
#include <string>
#include "Instance.h"


/**
 * sample test cases for CS249A aut11-12 students
 * author: anant bhardwaj
 * date: 11/7/2011
 * 
 */

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
    fleet->attributeIs("Truck, speed", "70");

    fleet->attributeIs("Boat, cost", "30");
    fleet->attributeIs("Truck, cost", "50");

    cout << "fleet->attribute('Boat, speed'): " << fleet->attribute("Boat, speed") << endl;
    cout << "fleet->attribute('Truck, speed'): " << fleet->attribute("Truck, speed") << endl;
    cout << "fleet->attribute('Boat, cost'): " << fleet->attribute("Boat, cost") << endl;
    cout << "fleet->attribute('Truck, cost'): " << fleet->attribute("Truck, cost") << endl;

    // --- Create instances
    // -- Locations
    // customers
    Ptr<Instance> customer1 = manager->instanceNew("customer1", "Customer");

    // ports
    Ptr<Instance> port1 = manager->instanceNew("port1", "Port");
    Ptr<Instance> port2 = manager->instanceNew("port2", "Port");
    Ptr<Instance> port3 = manager->instanceNew("port3", "Port");

    // Truck Terminal
    Ptr<Instance> truckTerminal1 = manager->instanceNew("truckterminal1", "Truck terminal");

    // Boat Terminal
    Ptr<Instance> boatTerminal1 = manager->instanceNew("boatterminal1", "Boat terminal");

    if (customer1 == NULL || port1 == NULL || port2 == NULL || port3 == NULL
        || truckTerminal1 == NULL || boatTerminal1 == NULL) {
        cerr << "Unexpected NULL customer or port." << endl;
    return 1;
    }

    // -- Segments
    // boat
    Ptr<Instance> boatSeg1 = manager->instanceNew("boatSeg1", "Boat segment");
    Ptr<Instance> boatSeg2 = manager->instanceNew("boatSeg2", "Boat segment");
    Ptr<Instance> boatSeg3 = manager->instanceNew("boatSeg3", "Boat segment");
    Ptr<Instance> boatSeg4 = manager->instanceNew("boatSeg4", "Boat segment");
    Ptr<Instance> boatSeg5 = manager->instanceNew("boatSeg5", "Boat segment");
    Ptr<Instance> boatSeg6 = manager->instanceNew("boatSeg6", "Boat segment");
    /*Ptr<Instance> boatSeg7 = manager->instanceNew("boatSeg6", "Boat segment");
    if (boatSeg7 != NULL) {
        cout << "Creating instance with same name -- error \n";
    }*/
    // truck
    Ptr<Instance> truckSeg1 = manager->instanceNew("truckSeg1", "Truck segment");
    Ptr<Instance> truckSeg2 = manager->instanceNew("truckSeg2", "Truck segment");
    Ptr<Instance> truckSeg3 = manager->instanceNew("truckSeg3", "Truck segment");
    Ptr<Instance> truckSeg4 = manager->instanceNew("truckSeg4", "Truck segment");
    Ptr<Instance> truckSeg5 = manager->instanceNew("truckSeg5", "Truck segment");
    Ptr<Instance> truckSeg6 = manager->instanceNew("truckSeg6", "Truck segment");
    Ptr<Instance> truckSeg7 = manager->instanceNew("truckSeg7", "Truck segment");
    Ptr<Instance> truckSeg8 = manager->instanceNew("truckSeg8", "Truck segment");

    if (boatSeg1 == NULL || boatSeg2 == NULL || truckSeg1 == NULL || truckSeg2 == NULL) {
        cerr << "Unexpected NULL segment." << endl;
        return 1;
    }

    // connections
    // customer1 <---> port1
    truckSeg1->attributeIs("source", "customer1");
    truckSeg2->attributeIs("source", "port1");
    truckSeg1->attributeIs("return segment", "truckSeg2");
    cout << "truckSeg1->attribute('source'): " << truckSeg1->attribute("source") << endl;
    cout << "truckSeg2->attribute('source'): " << truckSeg2->attribute("source") << endl;
    cout << "truckSeg1->attribute('return segment'): " << truckSeg1->attribute("return segment") << endl;
    cout << "truckSeg2->attribute('return segment'): " << truckSeg2->attribute("return segment") << endl;

    // customer1 <---> truck terminal 1
    truckSeg3->attributeIs("source", "port1");
    truckSeg4->attributeIs("source", "truckterminal1");
    truckSeg3->attributeIs("return segment", "truckSeg4");
    cout << "truckSeg3->attribute('source'): " << truckSeg3->attribute("source") << endl;
    cout << "truckSeg4->attribute('source'): " << truckSeg4->attribute("source") << endl;
    cout << "truckSeg3->attribute('return segment'): " << truckSeg3->attribute("return segment") << endl;
    cout << "truckSeg4->attribute('return segment'): " << truckSeg4->attribute("return segment") << endl;

    // truck terminal 1 <---> port 2
    truckSeg5->attributeIs("source", "truckterminal1");
    truckSeg6->attributeIs("source", "port2");
    truckSeg5->attributeIs("return segment", "truckSeg6");
    cout << "truckSeg5->attribute('source'): " << truckSeg5->attribute("source") << endl;
    cout << "truckSeg6->attribute('source'): " << truckSeg6->attribute("source") << endl;
    cout << "truckSeg5->attribute('return segment'): " << truckSeg5->attribute("return segment") << endl;
    cout << "truckSeg6->attribute('return segment'): " << truckSeg6->attribute("return segment") << endl;

    // truck terminal 1 <---> port 3
    truckSeg7->attributeIs("source", "truckterminal1");
    truckSeg8->attributeIs("source", "port3");
    truckSeg7->attributeIs("return segment", "truckSeg8");
    cout << "truckSeg7->attribute('source'): " << truckSeg7->attribute("source") << endl;
    cout << "truckSeg8->attribute('source'): " << truckSeg8->attribute("source") << endl;
    cout << "truckSeg7->attribute('return segment'): " << truckSeg7->attribute("return segment") << endl;
    cout << "truckSeg8->attribute('return segment'): " << truckSeg8->attribute("return segment") << endl;

    
    // port2 <---> boat terminal 1
    boatSeg1->attributeIs("source", "port2");
    boatSeg2->attributeIs("source", "boatterminal1");
    boatSeg1->attributeIs("return segment", "boatSeg2");
    cout << "boatseg1->attribute('source'): " << boatSeg1->attribute("source") << endl;
    cout << "boatseg2->attribute('source'): " << boatSeg2->attribute("source") << endl;
    cout << "boatseg1->attribute('return segment'): " << boatSeg1->attribute("return segment") << endl;
    cout << "boatseg2->attribute('return segment'): " << boatSeg2->attribute("return segment") << endl;


    // port3 <---> boat terminal 1
    boatSeg3->attributeIs("source", "boatterminal1");
    boatSeg4->attributeIs("source", "port3");
    boatSeg3->attributeIs("return segment", "boatSeg4");
    cout << "boatseg3->attribute('source'): " << boatSeg3->attribute("source") << endl;
    cout << "boatseg4->attribute('source'): " << boatSeg4->attribute("source") << endl;
    cout << "boatseg3->attribute('return segment'): " << boatSeg3->attribute("return segment") << endl;
    cout << "boatseg4->attribute('return segment'): " << boatSeg4->attribute("return segment") << endl;


    //  boat terminal 1 <---> customer1
    boatSeg5->attributeIs("source", "boatterminal1");
    boatSeg6->attributeIs("source", "customer1");
    boatSeg5->attributeIs("return segment", "boatSeg6");
    cout << "boatseg5->attribute('source'): " << boatSeg5->attribute("source") << endl;
    cout << "boatseg6->attribute('source'): " << boatSeg6->attribute("source") << endl;
    cout << "boatseg5->attribute('return segment'): " << boatSeg5->attribute("return segment") << endl;
    cout << "boatseg6->attribute('return segment'): " << boatSeg6->attribute("return segment") << endl;

    

    // -- Segment lengths
    boatSeg1->attributeIs("length", "400");
    boatSeg2->attributeIs("length", "300");
    boatSeg3->attributeIs("length", "200");
    boatSeg4->attributeIs("length", "500");
    boatSeg5->attributeIs("length", "600");
    boatSeg6->attributeIs("length", "700");
    truckSeg1->attributeIs("length", "800");
    truckSeg2->attributeIs("length", "900");
    truckSeg3->attributeIs("length", "200");
    truckSeg4->attributeIs("length", "400");
    truckSeg5->attributeIs("length", "500");
    truckSeg6->attributeIs("length", "600");
    truckSeg7->attributeIs("length", "800");
    truckSeg8->attributeIs("length", "1000");

    // -- Segment difficulties
    boatSeg1->attributeIs("difficulty", "1");
    boatSeg2->attributeIs("difficulty", "1.4");
    boatSeg3->attributeIs("difficulty", "1.8");
    boatSeg4->attributeIs("difficulty", "2");
    boatSeg5->attributeIs("difficulty", "4");
    boatSeg6->attributeIs("difficulty", "3");
    truckSeg1->attributeIs("difficulty", "2");
    truckSeg2->attributeIs("difficulty", "3");
    truckSeg3->attributeIs("difficulty", "4.5");
    truckSeg4->attributeIs("difficulty", "5");
    truckSeg5->attributeIs("difficulty", "1");
    truckSeg6->attributeIs("difficulty", "1.5");
    truckSeg7->attributeIs("difficulty", "1.5");
    truckSeg8->attributeIs("difficulty", "1.7");

    // -- Segment expedite support
    boatSeg1->attributeIs("expedite support", "yes");
    boatSeg2->attributeIs("expedite support", "yes");
    boatSeg3->attributeIs("expedite support", "yes");
    boatSeg4->attributeIs("expedite support", "yes");
    boatSeg5->attributeIs("expedite support", "yes");
    boatSeg6->attributeIs("expedite support", "yes");
    truckSeg1->attributeIs("expedite support", "yes");
    truckSeg2->attributeIs("expedite support", "yes");
    truckSeg3->attributeIs("expedite support", "yes");
    truckSeg4->attributeIs("expedite support", "yes");
    truckSeg5->attributeIs("expedite support", "yes");
    truckSeg6->attributeIs("expedite support", "yes");
    truckSeg7->attributeIs("expedite support", "yes");
    truckSeg8->attributeIs("expedite support", "yes");

    

    // -- Connectivity queries
    Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");

    if (conn == NULL) {
        cerr << "Unexpected NULL conn." << endl;
        return 1;
    }

    cout << "**** explore customer1 : expedited****" << endl;
    cout << conn->attribute("explore customer1 : expedited distance 10000 time 1000") << endl;
    cout << endl;

    cout << "*** connect port3 : truckterminal1 ****" << endl;
    cout << conn->attribute("connect port3 : truckterminal1") << endl;
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
