#ifndef __ACTIVITY_REACTOR_H__
#define __ACTIVITY_REACTOR_H__

#include "ActivityImpl.h"
    
class ActivityReactor : public Activity::Notifiee {
 	public:
		ActivityReactor(Fwk::Ptr<Activity::Manager> manager, Activity* activity) : 
		Notifiee(activity), activity_(activity), manager_(manager) {}
		void onNextTime();
    void onStatus();

	protected:
    Activity::Ptr activity_;
    Fwk::Ptr<Activity::Manager> manager_;	
};
    
#endif