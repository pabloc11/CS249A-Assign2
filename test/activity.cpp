#include <iostream>
#include "Instance.h"
#include "Activity.h"
#include "ActivityReactor.h"

int main(int argc, char* argv[]) {
	
	Ptr<Instance::Manager> shippingManager = shippingInstanceManager();
	Ptr<Activity::Manager> activityManager = shippingManager->activityManager();

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

