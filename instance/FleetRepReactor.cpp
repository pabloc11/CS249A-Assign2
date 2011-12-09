#include "Engine.h"
#include "Reactors.h"

namespace Shipping {
	
FleetRepReactor::FleetRepReactor(Fleet::Ptr _fleet) :
	fleet_(_fleet),
	scheduledAttrs_(),
	scheduleReactor_(NULL),
	unscheduleReactor_(NULL)
{}

double FleetRepReactor::currentTimeRoundedDownTo24() {
	double now = activityManagerInstance()->now().value();
	double remainder = fmod(now,24.0);
	return now - remainder;
}

void FleetRepReactor::onAttributeIs() {
	
	// Create scheduleChanges activity
	if(scheduleReactor_)
		activityManagerInstance()->activityDel("schedule");
	Activity::Ptr scheduleChanges = activityManagerInstance()->activityNew("schedule");
	scheduleReactor_ = new ScheduleChangesReactor(scheduleChanges.ptr(), fleet_, scheduledAttrs_);
	scheduleChanges->nextTimeIs(currentTimeRoundedDownTo24() + scheduledAttrs_.startTime.value());
	scheduleChanges->lastNotifieeIs(scheduleReactor_.ptr());
	scheduleChanges->statusIs(Activity::queued);
	
	// Create unscheduleChanges activity
	if(unscheduleReactor_)
		activityManagerInstance()->activityDel("unschedule");
	Activity::Ptr unscheduleChanges = activityManagerInstance()->activityNew("unschedule");
	unscheduleReactor_ = new UnscheduleChangesReactor(unscheduleChanges.ptr(), fleet_, scheduledAttrs_);
	unscheduleChanges->nextTimeIs(currentTimeRoundedDownTo24() + scheduledAttrs_.endTime.value());
	unscheduleChanges->lastNotifieeIs(unscheduleReactor_.ptr());
	unscheduleChanges->statusIs(Activity::queued);

}
}
