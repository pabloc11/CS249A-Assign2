#include <iostream>
#include <time.h>
#include "ActivityImpl.h"

Fwk::Ptr<Activity::Manager> activityManagerInstance() {
  return ActivityImpl::ManagerImpl::activityManagerInstance();
}

namespace ActivityImpl {
	
//Definition of static member
Fwk::Ptr<Activity::Manager> ManagerImpl::activityInstance_ = NULL;

//Gets the singleton instance of ManagerImpl
Fwk::Ptr<Activity::Manager> ManagerImpl::activityManagerInstance() {

	if (activityInstance_ == NULL) {
	  activityInstance_ = new ManagerImpl();
	}

	return activityInstance_;
}
  
Activity::Ptr ManagerImpl::activityNew(const string& name) {

	Activity::Ptr activity = activities_[name];
	if (activity != NULL) {
	  cerr << "Activity already exists!" << endl;
	  return NULL;
	}	
	activity = new ActivityImpl(name, this);
	activities_[name] = activity;

	return activity;
}

Activity::Ptr ManagerImpl::activity(const string& name) const {

	map<string, Activity::Ptr>::const_iterator it = activities_.find(name);
	if (it != activities_.end()) {
	  return (*it).second;
	}
	// dont throw an exception (accessor)
	return NULL; 
}
  
void ManagerImpl::activityDel(const string& name) {
	activities_.erase(name);
}
  
void ManagerImpl::lastActivityIs(Activity::Ptr activity) {
	scheduledActivities_.push(activity);
}

void ManagerImpl::nowIs(Time t) {

	//find the most recent activites to run and run them in order
	while (!scheduledActivities_.empty()) {

		cout << "Size of activities queue: " << scheduledActivities_.size() << endl;
   
	  //figure out the next activity to run
	  Activity::Ptr nextToRun = scheduledActivities_.top();
		scheduledActivities_.pop();

	  //if the next time is greater than the specified time, break the loop
   	if (nextToRun->nextTime() > t) {
			break;
	  }
   
 		//calculate amount of time to sleep
		Time diff = Time(nextToRun->nextTime().value() - now_.value());

		//sleep 100ms (100,000 microseconds) for every unit of time
		//usleep(( ((int)diff.value()) * 100000));

		now_ = nextToRun->nextTime();

		//run the minimum time activity and remove it from the queue
		nextToRun->statusIs(Activity::executing);
		nextToRun->statusIs(Activity::done);
	}

	//syncrhonize the time
	now_ = t;
}

}