#ifndef __ACTIVITY_IMPL_H__
#define __ACTIVITY_IMPL_H__z

#include <map>
#include <queue>
#include "Activity.h"

Fwk::Ptr<Activity::Manager> activityManagerInstance();

namespace ActivityImpl {
	
class ActivityComp : public binary_function<Activity::Ptr, Activity::Ptr, bool>
{
	public:
		ActivityComp() {}
  	bool operator() (Activity::Ptr lhs, Activity::Ptr rhs) const {	
			return lhs->nextTime() > rhs->nextTime();
  	}
};

class ActivityImpl : public Activity
{
  protected:
  	ActivityImpl(const string& name, Fwk::Ptr<class ManagerImpl> manager) :
				Activity(name), manager_(manager), status_(queued), nextTime_(0.0), notifiee_(NULL) {}
    
		Fwk::Ptr<class ManagerImpl> manager() const { return manager_; }

		virtual Status status() const { return status_; }
		virtual void statusIs(Status s) {
    	status_ = s;
    	if (notifiee_ != NULL) {
				notifiee_->onStatus();
    	}
		}

		virtual Time nextTime() const { return nextTime_; }
		virtual void nextTimeIs(Time t) {
    	nextTime_ = t;
		}

		virtual Notifiee::Ptr notifiee() const { return notifiee_; }
		virtual void lastNotifieeIs(Notifiee* n) {
    	ActivityImpl* me = const_cast<ActivityImpl*>(this);
    	me->notifiee_ = n;
		}

  private:
    friend class ManagerImpl;
    Fwk::Ptr<class ManagerImpl> manager_;
    Status status_;
		Time nextTime_;
		Notifiee* notifiee_;
};

class ManagerImpl : public Activity::Manager
{
  public:
    typedef Fwk::Ptr<ManagerImpl> Ptr;
		
	  virtual Activity::Ptr activityNew(const string& name);
	  virtual Activity::Ptr activity(const string& name) const;
	  virtual void activityDel(const string& name);

	  virtual Time now() const { return now_; }
	  virtual void nowIs(Time time);

	  virtual Type type() const { return type_; };
	  virtual void typeIs(Type _type) { type_ = _type; };

	  static Fwk::Ptr<Activity::Manager> activityManagerInstance();

	  virtual void lastActivityIs(Activity::Ptr activity);
  
	protected:
    ManagerImpl() : now_(0) {}

    //Data members
    priority_queue<Activity::Ptr, vector<Activity::Ptr>, ActivityComp> scheduledActivities_;
    map<string, Activity::Ptr> activities_; //pool of all activities
    Time now_;
    Type type_;

    //singleton instance
    static Fwk::Ptr<Activity::Manager> activityInstance_;
};

}

#endif /* __ACTIVITY_IMPL_H__ */

