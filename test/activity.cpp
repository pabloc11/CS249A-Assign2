#include <iostream>
#include "Instance.h"
#include "Activity.h"
#include "ActivityReactor.h"

int main(int argc, char* argv[]) {
	
	Ptr<Instance::Manager> manager = shippingInstanceManager();
	manager->instanceNew("clock", "Clock");
	Ptr<Activity::Manager> activityManager = manager->activityManager();
	
	/* Set up the network */
	vector< Ptr<Instance> > loc;
	vector< Ptr<Instance> > seg;

	// Locations
	loc.push_back( manager->instanceNew("customer1", "Customer") );
	loc.push_back( manager->instanceNew("customer2", "Customer") );

	// Segments
    seg.push_back( manager->instanceNew("ts1", "Truck segment") );
	seg.push_back( manager->instanceNew("ts2", "Truck segment") );

	// Connections...
	seg[0]->attributeIs("source", "customer1");
	seg[1]->attributeIs("source", "customer2");
	seg[0]->attributeIs("return segment", "ts2");

    Ptr<Instance> fleet = manager->instanceNew("Fleet", "Fleet");
    fleet->attributeIs("Truck, speed", "1");
    fleet->attributeIs("Truck, capacity", "1");

	// Set some segment (shipment) capacities
	seg[0]->attributeIs("capacity", "1");
	seg[1]->attributeIs("capacity", "1");

	Ptr<Instance> conn = manager->instanceNew("myConn", "Conn");
	conn->attributeIs("routing algorithm", "dfs");

	activityManager->nowIs(6.0);

	// Stop injection activity from the first two customers
	loc[0]->attributeIs("transfer rate", "1");

	activityManager->nowIs(24.0);

/*
	// Create activities
  Activity::Ptr activity1 = manager->activityNew("activity 1");
  Activity::Ptr activity2 = manager->activityNew("activity 2");
  Activity::Ptr activity3 = manager->activityNew("activity 3");

	// Create activity reactors
  activity1->lastNotifieeIs(new ActivityReactor(manager, activity1.ptr())); 
  activity2->lastNotifieeIs(new ActivityReactor(manager, activity2.ptr())); 
  activity3->lastNotifieeIs(new ActivityReactor(manager, activity3.ptr())); 

	// Add activities to activity manager queue
	manager->lastActivityIs(activity1);
	manager->lastActivityIs(activity2);
	manager->lastActivityIs(activity3);

	// Set activity start times
	activity1->nextTimeIs(0.0);
	activity2->nextTimeIs(2.0);
	activity3->nextTimeIs(1.0);
  
	// Start simulation
	manager->nowIs(20.0);
*/
  return 0;
}

