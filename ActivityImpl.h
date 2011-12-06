#ifndef __ACTIVITY_IMPL_H__
#define __ACTIVITY_IMPL_H__

#include <map>
#include <queue>
#include "Activity.h"

class ManagerImpl;

class ActivityImpl : public Activity
{
	public:
		ActivityImpl(const string& name) : Activity(name) {}
		
		Status status() const {return status_;}
		void statusIs(Status s);
	
		Time nextTime() const {return nextTime_;}
		void nextTimeIs(Time t);
	
		Fwk::Ptr<Notifiee> notifiee() const {return lastNotifiee_;}
		void lastNotifieeIs(Notifiee* n);

	private:
		Fwk::Ptr<Manager> manager_;
		enum Status status_;
		Time nextTime_;
		Fwk::Ptr<Notifiee> lastNotifiee_;
};

class ManagerImpl : public Activity::Manager
{
	public:
		Fwk::Ptr<Activity> activityNew(const string &name);
		Fwk::Ptr<Activity> activity(const string &name) const;
	
		void activityDel(const string &name);
		void lastActivityIs(Activity::Ptr);
   
		Time now() const {return now_;}
		void nowIs(Time);
		
	private:
		Time now_;
		map<string, Activity::Ptr> activities_;
		priority_queue<Activity::Ptr, vector<Activity::Ptr>, ActivityComparator> scheduledActivities_;
};


#endif /* __ACTIVITY_IMPL_H__ */

