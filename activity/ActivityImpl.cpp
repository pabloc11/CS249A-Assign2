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
	  cerr << "Activity: " << name << " already exists." << endl;
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
	vector<Activity::Ptr> allActivities;
	while(!scheduledActivities_.empty()) {
		Activity::Ptr cur = scheduledActivities_.top();
		scheduledActivities_.pop();
		if(cur->name() == name)
			break;
		allActivities.push_back(cur);
	}
	while(!allActivities.empty()) {
		scheduledActivities_.push(allActivities.back());
		allActivities.pop_back();
	}
}
  
void ManagerImpl::lastActivityIs(Activity::Ptr activity) {
	scheduledActivities_.push(activity);
}

void ManagerImpl::nowIs(Time t) {

	//find the most recent activites to run and run them in order
	while (!scheduledActivities_.empty()) {
   
	  //figure out the next activity to run
	  Activity::Ptr nextToRun = scheduledActivities_.top();
		scheduledActivities_.pop();

	  //if the next time is greater than the specified time, break the loop
   	if (nextToRun->nextTime() > t) {
			break;
	  }
	
		//cout << "Running activity: " << nextToRun->name() + " at time: " << nextToRun->nextTime().value() << endl;
   
 		//calculate amount of time to sleep
		Time diff = Time(nextToRun->nextTime().value() - now_.value());

		//sleep 1s for every unit of time
		if(type_ == real_)
			usleep(( ((int)diff.value()) * 1000000));

		now_ = nextToRun->nextTime();
		
		//run the minimum time activity
		nextToRun->statusIs(Activity::executing);
	}

	//syncrhonize the time
	now_ = t;
}

}
