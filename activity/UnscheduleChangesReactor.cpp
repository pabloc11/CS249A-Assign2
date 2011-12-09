#include "Activity.h"
#include "Reactors.h"

UnscheduleChangesReactor::UnscheduleChangesReactor(Activity* _a, Fleet::Ptr _f, ScheduledFleetAttrs& _s) : 
	Activity::Notifiee(_a),
	activity_(_a),
	fleet_(_f),
	scheduledAttrs_(_s) {}

void UnscheduleChangesReactor::onStatus() {
		
	if (activity_->status() == Activity::queued) {
		cout << "  Queueing " << activity_->name() << endl;
		activityManagerInstance()->lastActivityIs(activity_);
	}
		
  else if (activity_->status() == Activity::executing) {
	
		cout << "  Executing unscheduling activity" << endl;
	
		if (scheduledAttrs_.tSpeed) {
			fleet_->truckSpeedIs(scheduledAttrs_.truckSpeed);
		}
		if (scheduledAttrs_.bSpeed) {
			fleet_->boatSpeedIs(scheduledAttrs_.boatSpeed);
		}
		if (scheduledAttrs_.pSpeed) {
			fleet_->planeSpeedIs(scheduledAttrs_.planeSpeed);
		}
		if (scheduledAttrs_.tCost) {
			fleet_->truckCostIs(scheduledAttrs_.truckCost);
		}
		if (scheduledAttrs_.bCost) {
			fleet_->boatCostIs(scheduledAttrs_.boatCost);
		}
		if (scheduledAttrs_.pCost) {
			fleet_->planeCostIs(scheduledAttrs_.planeCost);
		}
		if (scheduledAttrs_.tCapacity) {
			fleet_->truckCapacityIs(scheduledAttrs_.truckCapacity);
		}
		if (scheduledAttrs_.bCapacity) {
			fleet_->boatCapacityIs(scheduledAttrs_.boatCapacity);
		}
		if (scheduledAttrs_.pCapacity) {
			fleet_->planeCapacityIs(scheduledAttrs_.planeCapacity);
		}	

		//activity_->nextTimeIs(activity_->nextTime().value() + 24.0);
		activity_->nextTimeIs(scheduledAttrs_.endTime.value() + FleetRepReactor::currentTimeRoundedDownTo24() + 24.0);
		activity_->statusIs(Activity::queued);
	}
}

