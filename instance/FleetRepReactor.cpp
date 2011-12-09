#include "Engine.h"
#include "Reactors.h"

namespace Shipping {
	
FleetRepReactor::FleetRepReactor(const string& _name, Fleet::Ptr _fleet) :
	name_(_name),
	fleet_(_fleet)
{}

void FleetRepReactor::onAttributeIs() {
	
	// Create scheduleChanges activity
	Activity::Ptr scheduleChanges = activityManagerInstance()->activityNew("schedule" + name_);
	ScheduleChangesReactor::Ptr scheduleReactor = new ScheduleChangesReactor(scheduleChanges.ptr(), fleet_, scheduledAttrs_);
	scheduleChanges->nextTimeIs(scheduledAttrs_.startTime.value());
	scheduleChanges->lastNotifieeIs(scheduleReactor.ptr());
	scheduleChanges->statusIs(Activity::queued);
	
	// Create unscheduleChanges activity
	Activity::Ptr unscheduleChanges = activityManagerInstance()->activityNew("unschedule" + name_);
	UnscheduleChangesReactor::Ptr unscheduleReactor = new UnscheduleChangesReactor(unscheduleChanges.ptr(), fleet_, scheduledAttrs_);
	unscheduleChanges->nextTimeIs(scheduledAttrs_.endTime.value());
	unscheduleChanges->lastNotifieeIs(unscheduleReactor.ptr());
	unscheduleChanges->statusIs(Activity::queued);
}

}